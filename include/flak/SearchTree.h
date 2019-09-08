/*

Copyright 2019 flak authors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#ifndef ALG_SEARCHTREE_H
#define ALG_SEARCHTREE_H

#include <iterator>
using std::pair;

namespace flak {

template<typename T>
struct SearchNode {
    typedef SearchNode<T> *NodePtr;
    typedef const SearchNode<T> *ConstNodePtr;

    T value_;
    NodePtr parent_;
    NodePtr left_;
    NodePtr right_;

    // find the minimum node
    static NodePtr minimum(NodePtr x) {
        while (x->left_) x = x->left_;
        return x;
    }

    // find the maximum node
    static NodePtr maximum(NodePtr x) {
        while (x->right_ != 0) x = x->right_;
        return x;
    }
};

template<typename T>
struct SearchTreeIterator {
    typedef T value_type;
    typedef T &reference;
    typedef T *pointer;
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef SearchTreeIterator<T> Self;
    typedef SearchNode<T> *NodePtr;

    NodePtr node_;
    NodePtr header_;

    SearchTreeIterator() : node_() {}

    SearchTreeIterator(NodePtr x, NodePtr header) : node_(x), header_(header) {}

    reference operator*() const { return node_->value_; }

    pointer operator->() const { return &(operator*()); }

    void increment() {
        if (node_->right_ != 0) {
            node_ = node_->right_;
            while (node_->left_ != 0) {
                node_ = node_->left_;
            }
        } else {
            NodePtr y = node_->parent_;
            while (node_ == y->right_) {
                node_ = y;
                y = y->parent_;
            }
            if (node_->right_ != y) {
                node_ = y;
            }
        }
    }

    void decrement() {
        if (node_ == header_) { // end() node
            node_ = node_->right_;
        } else if (node_->left_ != 0) {
            NodePtr y = node_->left_;
            while (y->right_ != 0) {
                y = y->right_;
            }
            node_ = y;
        } else {
            NodePtr y = node_->parent_;
            while (node_ == y->left_) {
                node_ = y;
                y = y->parent_;
            }
            node_ = y;
        }
    }

    Self &operator++() {
        increment();
        return *this;
    }

    Self operator++(int) {
        Self tmp = *this;
        increment();
        return tmp;
    }

    Self &operator--() {
        decrement();
        return *this;
    }

    Self operator--(int) {
        Self tmp = *this;
        decrement();
        return tmp;
    }

    bool operator==(const Self &x) { return node_ == x.node_; }

    bool operator!=(const Self &x) { return node_ != x.node_; }
};

template<typename Key, typename Val,
        typename KeyOfValue, typename Compare, typename Alloc=std::allocator<Val>>
class SearchTree {
public:
    typedef Key key_type;
    typedef Val value_type;
    typedef Val *pointer;
    typedef Val &reference;
    typedef const Val *const_pointer;
    typedef const Val &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef SearchNode<Val> *NodePtr;
    typedef SearchNode<Val> Node;
    typedef const NodePtr ConstNodePtr;

protected:
    typedef typename Alloc::template rebind<Val>::other ValueAlloc;
    typedef std::allocator_traits<ValueAlloc> ValueAllocTraits;
    typedef typename ValueAllocTraits::template rebind_alloc<Node> NodeAlloc;
    typedef std::allocator_traits<NodeAlloc> NodeAllocTraits;

    NodeAlloc nodeAlloc;

    NodePtr getNode() {
        return NodeAllocTraits::allocate(nodeAlloc, 1);
    }

    void putNode(NodePtr p) {
        NodeAllocTraits::deallocate(nodeAlloc, p, 1);
    }

    NodePtr createNode(const Val &x) {
        NodePtr tmp = getNode();
        NodeAllocTraits::construct(nodeAlloc, &(tmp->value_), x);
        return tmp;
    }

    void destroyNode(NodePtr p) {
        NodeAllocTraits::destroy(nodeAlloc, &(p->value_));
        NodeAllocTraits::destroy(nodeAlloc, p);
        putNode(p);
    }

private:
    Compare keyComp_;
    NodePtr header_;
    size_type nodeCount_;

protected:
    typedef SearchTree<Key, Val, KeyOfValue, Compare, Alloc> Self;

    NodePtr &root() const { return header_->parent_; }

    NodePtr &leftmost() const { return header_->left_; }

    NodePtr &rightmost() const { return header_->right_; }

    static NodePtr &left(NodePtr x) { return x->left_; }

    static NodePtr &right(NodePtr x) { return x->right_; }

    static NodePtr &parent(NodePtr x) { return x->parent_; }

    static reference value(NodePtr x) { return x->value_; }

    static Key &key(NodePtr x) { return KeyOfValue()(value(x)); }

    void init() {
        header_ = getNode();
        root() = nullptr;
        leftmost() = header_;    // point to head itself
        rightmost() = header_;
        nodeCount_ = 0;
    }

public:
    typedef SearchTreeIterator<Val> iterator;

    SearchTree() { init(); }

    SearchTree(const Compare &comp) : keyComp_(comp) { init(); }

    iterator begin() const { return iterator(leftmost(), header_); }

    iterator end() const { return iterator(header_, header_); }

    bool empty() const { return nodeCount_ == 0; }

    size_type size() const { return nodeCount_; }

    size_type max_size() const { return NodeAllocTraits::max_size(nodeAlloc); }

    void clear() {
        _eraseAll(root());
        leftmost() = header_;
        rightmost() = header_;
        root() = nullptr;
        nodeCount_ = 0;
    }

public:
    pair<iterator, bool> insertUnique(const Val &v) {
        NodePtr x = root(); // record the position to insert
        NodePtr y = header_; // record the parent of x
        bool comp = true;
        while (x != nullptr) {
            y = x;
            comp = keyComp_(KeyOfValue()(v), key(x));
            x = comp ? left(x) : right(x);
        }

        iterator j = iterator(y, header_);
        if (comp) {
            if (y == leftmost()) {
                return pair<iterator, bool>(_insert(x, y, v), true);
            } else {
                --j;
            }
        }

        if (keyComp_(key(j.node_), KeyOfValue()(v))) {
            return pair<iterator, bool>(_insert(x, y, v), true);
        }
        return pair<iterator, bool>(j, false);
    }

    iterator insertEqual(const Val &v) {
        NodePtr x = root(); // record the position to insert
        NodePtr y = header_; // record the parent of x
        bool comp = true;
        while (x != nullptr) {
            y = x;
            comp = keyComp_(KeyOfValue()(v), key(x));
            x = comp ? left(x) : right(x);
        }
        return _insert(x, y, v);
    }

    // find the last node being greater than or equal to [k]
    iterator lowerBound(const Key &k) {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            if (!keyComp_(key(x), k)) { // key(x) >= k
                y = x;
                x = left(x); // go left and find a smaller one
            } else {    // key(x) < k
                x = right(x);
            }
        }
        return iterator(y, header_);
    }

    // find the last node being greater than [k]
    iterator upperBound(const Key &k) {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            if (keyComp_(k, key(x))) { // k < key(x)
                y = x;
                x = left(x); // go left and find a smaller one
            } else {    // k >= key(x)
                x = right(x);
            }
        }
        return iterator(y, header_);
    }

    pair<iterator, iterator> equalRange(const Key &k) {
        return pair<iterator, iterator>(lowerBound(k), upperBound(k));
    }

    void erase(iterator x) {
        NodePtr y = _erase(x.node_);
        destroyNode(y);
        --nodeCount_;
    }

    void erase(iterator first, iterator last) {
        if (first == begin() && last == end()) {
            clear();
        } else {
            while (first != last) {
                // first will be deleted, so you cannot use the ++first or first++ in for loop.
                erase(first++);
            }
        }
    }

    size_type erase(const Key &k) {
        pair<iterator, iterator> p = equalRange(k);
        size_type old = size();
        erase(p.first, p.second);
        return old - size();
    }

    size_type count(const Key &k) {
        pair<iterator, iterator> p = equalRange(k);
        return (size_type) std::distance(p.first, p.second);
    }

    iterator find(const Key &k) {
        iterator j = lowerBound(k);
        return (j == end() || keyComp_(k, key(j.node_))) ? end() : j;
    }

    bool operator==(Self &x) {
        return size() == x.size() && std::equal(begin(), end(), x.begin(), x.end());
    }

    bool operator<(Self &x) {
        return std::lexicographical_compare(begin(), end(), x.begin(), x.end());
    }

    bool operator!=(Self &x) {
        return !(operator==(x));
    }

    bool operator>(Self &x) {
        return x < *this;
    }

    bool operator<=(Self &x) {
        return !(operator>(x));
    }

    bool operator>=(Self &x) {
        return !(operator<(x));
    }

private:

    // [x] is the position to insert
    // [p] is the parent of [x]
    iterator _insert(NodePtr x, NodePtr p, const Val &v) {
        bool insertLeft = (x != nullptr || p == header_
                           || keyComp_(KeyOfValue()(v), key(p)));

        NodePtr z = createNode(v);
        z->parent_ = p;
        z->left_ = nullptr;
        z->right_ = nullptr;

        if (insertLeft) {
            p->left_ = z;
            if (p == header_) {
                root() = z;
                header_->right_ = z;
            } else if (p == leftmost()) {
                leftmost() = z;
            }
        } else {
            p->right_ = z;
            if (p == rightmost()) {
                rightmost() = z;
            }
        }
        ++nodeCount_;
        return iterator(z, header_);
    }

    // erase node [z] and return the deleted node
    NodePtr _erase(NodePtr z) {
        NodePtr y = z;
        NodePtr x = nullptr;

        if (y->left_ == nullptr) {
            x = y->right_;
        } else if (y->right_ == nullptr) {
            x = y->left_;
        } else {    // z has two child
            y = y->right_;
            while (y->left_) {
                y = y->left_;
            }
            // If y has left, x is the successor of y
            // If y has no left, x is the successor of z
            // x may be nullptr
            x = y->right_;
        }

        if (y != z) {    // z has two child
            // y must be the right of z as above code
            z->left_->parent_ = y;
            y->left_ = z->left_;
            if (y != z->right_) {
                if (x) {
                    x->parent_ = y->parent_;
                }
                y->parent_->left_ = x;
                y->right_ = z->right_;
                z->right_->parent_ = y;
            }

            if (z == root()) {
                root() = y;
            } else if (z->parent_->left_ == z) {
                z->parent_->left_ = y;
            } else {
                z->parent_->right_ = y;
            }
            y->parent_ = z->parent_;
        } else {    // z has one child. y == z
            // so x is the successor
            if (x) {
                x->parent_ = z->parent_;
            }
            if (z == root()) {
                root() = x;
            } else if (z->parent_->left_ == z) {
                z->parent_->left_ = x;
            } else {
                z->parent_->right_ = x;
            }
            if (leftmost() == z) {
                if (z->right_ == nullptr) { // mean x is nullptr
                    leftmost() = z->parent_;
                } else { // mean x is the right of z
                    leftmost() = Node::minimum(x);
                }
            }
            if (rightmost() == z) {
                if (z->left_ == nullptr) {
                    rightmost() = z->parent_;
                } else {
                    rightmost() = Node::maximum(x);
                }
            }
        }
        return z;
    }

    void _eraseAll(NodePtr x) {
        while (x != nullptr) {
            _erase(right(x));
            NodePtr y = left(x);
            destroyNode(x);
            x = y;
        }
    }
};

}
#endif //ALG_SEARCHTREE_H
