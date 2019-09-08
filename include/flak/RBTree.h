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

#ifndef ALG_RBTREE_H
#define ALG_RBTREE_H

#include <cstddef>
#include <iterator>
#include <algorithm>
using std::bidirectional_iterator_tag;
using std::pair;

namespace flak {

enum RbTreeColor {
    rb_red = false, rb_black = true
};

template<typename T>
struct RBNode {
    typedef RBNode<T> *SelfPtr;
    typedef const RBNode<T> *ConstSelfPtr;

    RbTreeColor color_;
    SelfPtr parent_;
    SelfPtr left_;
    SelfPtr right_;
    T value_;

    // find the minimum node
    static SelfPtr minimum(SelfPtr x) {
        while (x->left_ != 0) x = x->left_;
        return x;
    }

    static ConstSelfPtr minimum(ConstSelfPtr x) {
        while (x->left_ != 0) x = x->left_;
        return x;
    }

    // find the maximum node
    static SelfPtr maximum(SelfPtr x) {
        while (x->right_ != 0) x = x->right_;
        return x;
    }

    static ConstSelfPtr maximum(ConstSelfPtr x) {
        while (x->right_ != 0) x = x->right_;
        return x;
    }
};

template<typename T, typename Ref, typename Ptr>
struct RBTreeIterator {
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef ptrdiff_t difference_type;
    typedef bidirectional_iterator_tag iterator_category;

    typedef RBTreeIterator<T, Ref, Ptr> Self;
    typedef RBNode<T> *NodePtr;

    NodePtr node_;

    explicit RBTreeIterator() = default;

    explicit RBTreeIterator(NodePtr x) : node_(x) {}

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
        if (node_->color_ == rb_red && node_->parent_->parent_ == node_) {
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

    reference operator*() const { return node_->value_; }

    pointer operator->() const { return &(node_->value_); }

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

    bool operator==(const Self &x) {
        return node_ == x.node_;
    }

    bool operator!=(const Self &x) {
        return node_ != x.node_;
    }
};

template<typename Key, typename Val, typename KeyOfValue,
        typename Compare,
        typename Alloc = std::allocator<Val> >
class RBTree {
public:
    typedef Val value_type;
    typedef Val *pointer;
    typedef const Val *const_pointer;
    typedef Val &reference;
    typedef const Val &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef typename Alloc::template rebind<Val>::other ValueAlloc;
    typedef std::allocator_traits<ValueAlloc> ValueAllocTraits;

    typedef typename ValueAllocTraits::template rebind_alloc<RBNode<Val>> NodeAlloc;
    typedef std::allocator_traits<NodeAlloc> NodeAllocTraits;

    typedef RBNode<Val> *NodePtr;
    typedef RBNode<Val> Node;
    typedef const NodePtr ConstNodePtr;
    typedef RbTreeColor ColorType;
    typedef RBTree<Key, Val, KeyOfValue, Compare, Alloc> Self;

protected:
    NodeAlloc nalloc;

    NodePtr getNode() {
        return NodeAllocTraits::allocate(nalloc, 1);
    }

    void putNode(NodePtr p) {
        NodeAllocTraits::deallocate(nalloc, p, 1);
    }

    NodePtr createNode(const Val &x) {
        NodePtr tmp = getNode();
        NodeAllocTraits::construct(nalloc, &(tmp->value_), x);
        return tmp;
    }

    void destroyNode(NodePtr p) {
        NodeAllocTraits::destroy(nalloc, &(p->value_));
        NodeAllocTraits::destroy(nalloc, p);
        putNode(p);
    }

    // clone a node
    NodePtr cloneNode(ConstNodePtr x) {
        NodePtr tmp = createNode(x->value_);
        tmp->color_ = x->color_;
        tmp->left_ = nullptr;
        tmp->right_ = nullptr;
        return tmp;
    }

protected:
    size_t nodeCount_;
    NodePtr header_;
    Compare keyCompare_;

    NodePtr &root() const { return header_->parent_; }

    NodePtr &leftmost() const { return header_->left_; }

    NodePtr &rightmost() const { return header_->right_; }

    static NodePtr &left(NodePtr x) {
        return x->left_;
    }

    static NodePtr &right(NodePtr x) {
        return x->right_;
    }

    static NodePtr &parent(NodePtr x) {
        return x->parent_;
    }

    static reference &value(NodePtr x) {
        return x->value_;
    }

    static const Key &key(NodePtr x) {
        return KeyOfValue()(value(x));
    }

    static ColorType &color(NodePtr x) {
        return x->color_;
    }

    static NodePtr minimum(NodePtr x) {
        return RBNode<Val>::minimum(x);
    }

    static NodePtr maximum(NodePtr x) {
        return RBNode<Val>::minimum(x);
    }

public:
    typedef RBTreeIterator<value_type, reference, pointer> iterator;
    typedef RBTreeIterator<value_type, const_reference, const_pointer> const_iterator;

private:
    void init() {
        header_ = getNode();
        color(header_) = rb_red;
        root() = nullptr;
        leftmost() = header_;    // point to head itself
        rightmost() = header_;
    }

public:
    explicit RBTree(const Compare &comp = Compare())
            : nodeCount_(0), keyCompare_(comp) { init(); }

    ~RBTree() {
        clear();
        putNode(header_);
    }

    Compare key_comp() const { return keyCompare_; }

    iterator begin() { return iterator(leftmost()); } // leftmost is begin()
    const_iterator begin() const { return const_iterator(leftmost()); }

    iterator end() { return iterator(header_); }

    const_iterator end() const { return const_iterator(header_); } // header_ is end()
    bool empty() { return nodeCount_ == 0; }

    size_t size() const { return nodeCount_; }

    size_t max_size() const { return size_type(-1); }

    void clear() {
        if (nodeCount_ != 0) {
            _erase(root());
            leftmost() = header_;
            root() = nullptr;
            rightmost() = header_;
            nodeCount_ = 0;
        }
    }

    size_type count(const Key& k) const {
        pair<const_iterator, const_iterator> p = equalRange(k);
        size_type n = std::distance(p.first, p.second);
        return n;
    }

private:
    /*
     *        x              y
     *       / \            / \
     *      u   y    =>    x   b
     *         / \        / \
     *        a   b      u   a
     *
     */
    // rotate left
    inline void _rotate_left(NodePtr x, NodePtr &root) {
        NodePtr y = x->right_;
        x->right_ = y->left_;
        if (y->left_ != 0) {
            y->left_->parent_ = x;
        }
        y->parent_ = x->parent_;

        if (x == root) {
            root = y;
        } else if (x->parent_->right_ == x) {
            x->parent_->right_ = y;
        } else {
            x->parent_->left_ = y;
        }
        y->left_ = x;
        x->parent_ = y;
    }

    /*
     *         x             y
     *        / \           / \
     *       y   u    =>   a   x
     *      / \               / \
     *     a   b             b   u
     */
    // rotate right
    inline void _rotate_right(NodePtr x, NodePtr &root) {
        NodePtr y = x->left_;
        x->left_ = y->right_;
        if (y->right_ != 0) {
            y->right_->parent_ = x;
        }
        y->parent_ = x->parent_;

        if (x == root) {
            root = y;
        } else if (x->parent_->right_ == x) {
            x->parent_->right_ = y;
        } else {
            x->parent_->left_ = y;
        }
        y->right_ = x;
        x->parent_ = y;
    }

    /*
     *         v
     *        / \
     *       a   u
     *      / \           a is a left node
     *     b   c          b is outside, c is inside
     *
     *        v
     *       / \
     *      u   a
     *         / \       a is a right node
     *        c   b      b is outside, c is inside
     */
    // Rebalance the tree
    inline void _rebalance(NodePtr x, NodePtr &root) {
        x->color_ = rb_red;
        while (x != root && x->parent_->color_ == rb_red) {  // rebalance only when parent is red
            if (x->parent_ == x->parent_->parent_->left_) {  // father is a left node
                NodePtr y = x->parent_->parent_->right_;    // consider uncle node
                if (y && y->color_ == rb_red) {  // uncle is red
                    // father and uncle change to black
                    x->parent_->color_ = rb_black;
                    y->color_ = rb_black;
                    // grandfather change to red
                    x->parent_->parent_->color_ = rb_red;
                    x = x->parent_->parent_; // percolate up
                } else {    // uncle is black or is null
                    if (x == x->parent_->right_) {   // x is inside
                        x = x->parent_;
                        _rotate_left(x, root); // rotate left
                    }
                    // father's color become as same as uncle's
                    x->parent_->color_ = rb_black;
                    x->parent_->parent_->color_ = rb_red;
                    _rotate_right(x->parent_->parent_, root); // rotate right
                }
            } else {    // father is a right node
                NodePtr y = x->parent_->parent_->left_; // uncle
                if (y && y->color_ == rb_red) {  // uncle is red
                    // father and uncle change to black
                    x->parent_->color_ = rb_black;
                    y->color_ = rb_black;
                    x->parent_->parent_->color_ = rb_red;
                } else {    // // uncle is black or is null
                    if (x == x->parent_->left_) {   // x is inside
                        x = x->parent_;
                        _rotate_right(x, root); // rotate left
                    }
                    x->parent_->color_ = rb_black;
                    x->parent_->parent_->color_ = rb_red;
                    _rotate_left(x->parent_->parent_, root);
                }
            }
        }
        // ensure root is black
        root->color_ = rb_black;
    }

    // x is the position need to be inserted
    // y is the father of x
    // v is the new value
    iterator _insert(NodePtr x, NodePtr y, const Val &v) {
        NodePtr z;

        if (y == header_
            || x != nullptr
            || keyCompare_(KeyOfValue()(v), key(y))) { // insert to left of y
            z = createNode(v);
            left(y) = z;
            if (y == header_) {
                root() = z;
                rightmost() = z;
            } else if (y == leftmost()) {
                leftmost() = z;
            }
        } else {
            z = createNode(v);
            right(y) = z;
            if (y == rightmost()) {
                rightmost() = z;
            }
        }
        parent(z) = y;
        left(z) = nullptr;
        right(z) = nullptr;

        // rebalance the tree
        _rebalance(z, root());
        ++nodeCount_;
        return iterator(z);
    }

    // private erase function without rebalancing.
    // it use postorder traversal.
    void _erase(NodePtr x) {
        while (x != nullptr) {
            _erase(right(x));
            NodePtr y = left(x);
            destroyNode(x);
            x = y;
        }
    }

public:
    // allow replaced key
    iterator insertEqual(const Val &v) {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            y = x;
            // if v is smaller than x->right_, return true
            x = keyCompare_(KeyOfValue()(v), key(x)) ? left(x) : right(x);
        }
        return _insert(x, y, v);
    }

    // not allow replaced key
    // three considerations:
    //  1. insert to the left of leftmost
    //  2. insert to the right of y if bigger than y
    //  3. insert to the left of y if bigger than decrement(y)
    pair<iterator, bool> insertUnique(const Val &v) {
        NodePtr y = header_;
        NodePtr x = root();
        bool comp = true;
        while (x != nullptr) {
            y = x;
            comp = keyCompare_(KeyOfValue()(v), key(x));
            x = comp ? left(x) : right(x);
        }
        // while end, y will be the parent of the node to insert

        iterator j = iterator(y);
        if (comp) { // comp is true, insert to left
            if (j == begin()) {
                return pair<iterator, bool>(_insert(x, y, v), true);
            } else { // if j is not the leftmost
                --j;
                // need to --j, because KeyOfValue(v) probably equal to --j,
                // it need to be bigger than --j as the following comparsion.
            }
        }

        // if v's node need to be inserted to y's right,
        // KeyofValue(v) need to bigger than y.
        // if v's node need to be inserted to y's left,
        // KeyofValue(v) need to bigger than decrement(y).
        if (keyCompare_(key(j.node_), KeyOfValue()(v))) {
            return pair<iterator, bool>(_insert(x, y, v), true);
        }
        return pair<iterator, bool>(j, false);
    }

    template<class II>
    void insertUnique(II first, II last) {
        for (; first != last; ++first) {
            insertUnique(*first);
        }
    }

    void insertUnique(iterator first, iterator last) {
        for (; first != last; ++first) {
            insertUnique(*first);
        }
    }

    void insertUnique(const_iterator first, const_iterator last) {
        for (; first != last; ++first) {
            insertUnique(*first);
        }
    }

    // input pointer
    void insertUnique(pointer first, pointer last) {
        for (; first != last; ++first) {
            insertUnique(*first);
        }
    }

    // Insert value to specific pos.
    // Actually, you can always use insertUnique(), because the pos is not to be trust.
    // You can do some improvement in a few scenes.
    iterator insertUnique(iterator pos, const Val &x) {
        if (pos.node_ == begin()) {  // begin()
            if (size() > 0 && keyCompare_(KeyOfValue(x), key(pos.node_))) {
                return _insert(pos.node_, pos.node_, x);
            } else {
                return insertUnique(x).first;
            }
        } else if (pos.node_ == end()) { // end()
            if (keyCompare_(key(rightmost()), KeyOfValue(x))) {
                return _insert(nullptr, pos.node_, x);  // insert to right
            } else {
                return insertUnique(x).first;
            }
        } else {
            iterator before = pos;
            --before;
            if (keyCompare_(key(before.node_), KeyOfValue(x))
                && keyCompare_(KeyOfValue(x), key(pos.node_))) {
                if (right(before.node_) == nullptr) {
                    return _insert(nullptr, pos.node_, x);
                } else {
                    return _insert(pos.node_, pos.node_, x);
                }
            } else {
                return insertUnique(x).first;
            }
        }
    }

    // find key.
    // If key(x) < k, go right,
    // else if key(x) >= k, go left.
    iterator find(const Key &k) {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            // !(key(x) < k) equal to key(x) >= k
            // actually, when k == key(x), y = x, the target node is y
            if (!keyCompare_(key(x), k)) {
                y = x;      // record the last key that bigger than or equal to k
                x = left(x);
            } else {    // key(x) < k
                x = right(x);
            }
        }

        // At Last,
        // If y is still header_, represent that k is too big, return end().
        // If y is not header_,
        //  as the code above, y will be equal or bigger than k.
        //  Only equal can return iterator j.
        //  If y bigger than k, return end().
        iterator j = iterator(y);
        return (j == end() || keyCompare_(k, key(j.node_))) ? end() : j;
    }

    const_iterator find(const Key &k) const {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            if (!keyCompare_(key(x), k)) {
                y = x;
                x = left(x);
            } else {
                x = right(x);
            }
        }
        const_iterator j = const_iterator(y);
        return (j == end() || keyCompare_(k, key(j.node_))) ? end() : j;
    }

    // erase one node
    void erase(iterator pos) {
        NodePtr y = _rebalanceForErase(pos.node_);
        destroyNode(y);
        --nodeCount_;
    }

    // erase a range
    void erase(iterator first, iterator last) {
        if (first == begin() && last == end()) {
            clear();
        } else {
            while (first != last) {
                erase(first++);
            }
        }
    }

    // erase a key
    // notice that you need to delete multi equal key.
    // return the number of deleted key.
    size_type erase(const Key &x) {
        pair<iterator, iterator> p = equalRange(x);
        size_type n = (size_type) std::distance(p.first, p.second);
        erase(p.first, p.second);
        return n;
    }

    // find the last node that is equal or bigger than k
    const_iterator lowerBound(const Key &k) const {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            if (!keyCompare_(key(x), k)) {
                y = x;
                x = left(x);    // key(x) still >= k, go on left
            } else {
                x = right(x);   // find a big one
            }
        }
        return const_iterator(y);
    }

    iterator lowerBound(const Key &k) {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            if (!keyCompare_(key(x), k)) {
                y = x;
                x = left(x);    // key(x) still >= k, go on left
            } else {
                x = right(x);   // find a big one
            }
        }
        return iterator(y);
    }


    // find the last node that is bigger than k (no equal)
    const_iterator upperBound(const Key &k) const {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            if (keyCompare_(k, key(x))) { // k < key(x)
                y = x;
                x = left(x);    // key(x) still > k, go on left
            } else {
                x = right(x);   // find a big one
            }
        }
        return const_iterator(y);
    }

    iterator upperBound(const Key &k) {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            if (keyCompare_(k, key(x))) {
                y = x;
                x = left(x);    // key(x) still > k, go on left
            } else {
                x = right(x);   // find a big one
            }
        }
        return iterator(y);
    }

    pair<iterator, iterator> equalRange(const Key &k) {
        return pair<iterator, iterator>(lowerBound(k), upperBound(k));
    }

    pair<const_iterator, const_iterator> equalRange(const Key &k) const {
        return pair<const_iterator, const_iterator>(lowerBound(k), upperBound(k));
    }

public:
    bool operator==(const Self &x) {
        return size() == x.size() && std::equal(begin(), end(), x.begin());
    }

    bool operator<(const Self &x) {
        return std::lexicographical_compare(begin(), end(), x.begin(), x.end());
    }

private:

    // erase and rebalance the tree.
    // Reference from SGI-STL
    inline NodePtr _rebalanceForErase(NodePtr z) {
        NodePtr y = z;
        NodePtr x = 0;  // x is the child of y
        NodePtr xParent = 0;

        if (y->left_ == 0)     // z has at most one non-null child. y == z.
            x = y->right_;    // x might be null.
        else if (y->right_ == 0)  // z has exactly one non-null child. y == z.
            x = y->left_;    // x is not null.
        else {               // z has two non-null children.  Set y to
            y = y->right_;   //   z's successor.  x might be null.
            while (y->left_ != 0)
                y = y->left_;
            x = y->right_;
        }

        if (y != z) {  // mean that z has two children
            z->left_->parent_ = y;
            y->left_ = z->left_;
            if (y != z->right_) {
                xParent = y->parent_;
                if (x) x->parent_ = y->parent_;
                y->parent_->left_ = x;      // y must be a child of left
                y->right_ = z->right_;
                z->right_->parent_ = y;
            } else {
                xParent = y;
            }

            if (root() == z)
                root() = y;
            else if (z->parent_->left_ == z)
                z->parent_->left_ = y;
            else
                z->parent_->right_ = y;

            y->parent_ = z->parent_;
            std::swap(y->color_, z->color_);
//            y->color_ = z->color_; // copy the color
            y = z;

        } else {   // mean that z has one child or none and y == z
            xParent = y->parent_;
            if (x) x->parent_ = y->parent_;
            if (root() == z)
                root() = x;
            else if (z->parent_->left_ == z)
                z->parent_->left_ = x;
            else
                z->parent_->right_ = x;
            if (leftmost() == z)
                if (z->right_ == nullptr)        // z->left must be null also
                    leftmost() = z->parent_;
                    // makes leftmost == header if z == root
                else
                    leftmost() = Node::minimum(x);
            if (rightmost() == z)
                if (z->left_ == nullptr)         // z->right must be null also
                    rightmost() = z->parent_;
                    // makes rightmost == header if z == root
                else                      // x == z->left
                    rightmost() = Node::maximum(x);
        }

        // rebalance
        if (y->color_ != rb_red) { // If successor y is red, it do not break the rule.
            while (x != root() && (x == nullptr || x->color_ == rb_black)) { // x is black now
                if (x == xParent->left_) {
                    // when x is a black, it must has a sibling
                    // because If x is a black with , it always is not a leaf
                    NodePtr w = xParent->right_; // sibling
                    if (w->color_ == rb_red) {
                        w->color_ = rb_black;
                        xParent->color_ = rb_red;
                        _rotate_left(xParent, root());
                        w = xParent->right_;
                    }
                    if ((w->left_ == 0 ||
                         w->left_->color_ == rb_black) &&
                        (w->right_ == 0 ||
                         w->right_->color_ == rb_black)) {
                        w->color_ = rb_red;
                        x = xParent;
                        xParent = xParent->parent_; // percolate up
                    } else {
                        if (w->right_ == 0 ||
                            w->right_->color_ == rb_black) {
                            if (w->left_) w->left_->color_ = rb_black;
                            w->color_ = rb_red;
                            _rotate_right(w, root());
                            w = xParent->right_;
                        }
                        w->color_ = xParent->color_;
                        xParent->color_ = rb_black;
                        if (w->right_) w->right_->color_ = rb_black;
                        _rotate_left(xParent, root());
                        break;
                    }
                } else {                  // same as above, with right <-> left.
                    NodePtr __w = xParent->left_;
                    if (__w->color_ == rb_red) {
                        __w->color_ = rb_black;
                        xParent->color_ = rb_red;
                        _rotate_right(xParent, root());
                        __w = xParent->left_;
                    }
                    if ((__w->right_ == 0 ||
                         __w->right_->color_ == rb_black) &&
                        (__w->left_ == 0 ||
                         __w->left_->color_ == rb_black)) {
                        __w->color_ = rb_red;
                        x = xParent;
                        xParent = xParent->parent_;
                    } else {
                        if (__w->left_ == 0 ||
                            __w->left_->color_ == rb_black) {
                            if (__w->right_) __w->right_->color_ = rb_black;
                            __w->color_ = rb_red;
                            _rotate_left(__w, root());
                            __w = xParent->left_;
                        }
                        __w->color_ = xParent->color_;
                        xParent->color_ = rb_black;
                        if (__w->left_) __w->left_->color_ = rb_black;
                        _rotate_right(xParent, root());
                        break;
                    }
                }
                if (x) x->color_ = rb_black;
            }
        }
        return z;
    }

};

}
#endif //ALG_RBTREE_H
