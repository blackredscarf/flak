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

#ifndef ALG_AVLTREE_H
#define ALG_AVLTREE_H

#include <cstddef>
#include <iterator>
#include <cassert>

using std::pair;

namespace flak {


template<typename T>
struct AVLNode {
    typedef AVLNode<T>* NodePtr;
    typedef const AVLNode<T>* ConstNodePtr;

    T value_;
    NodePtr parent_;
    NodePtr left_;
    NodePtr right_;
    signed char balFactor_;

    // find the minimum node
    static NodePtr minimum(NodePtr x) {
        while (x->left_) x = x->left_;
        return x;
    }

    static ConstNodePtr minimum(ConstNodePtr x) {
        while (x->left_) x = x->left_;
        return x;
    }

    // find the maximum node
    static NodePtr maximum(NodePtr x) {
        while (x->right_ != 0) x = x->right_;
        return x;
    }

    static ConstNodePtr maximum(ConstNodePtr x) {
        while (x->right_ != 0) x = x->right_;
        return x;
    }
};

template<typename T, typename Ref, typename Ptr>
struct AVLTreeIterator {
    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef AVLTreeIterator<T, Ref, Ptr> Self;
    typedef AVLNode<T>* NodePtr;
    NodePtr node_;

    AVLTreeIterator() : node_() {}

    explicit AVLTreeIterator(NodePtr x) : node_(x) {}

    reference operator*() const {
        return node_->value_;
    }

    pointer operator->() const {
        return &(operator*());
    }

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
        if (node_->parent_->parent_ == node_ && node_->balFactor_ == -2) { // end() node
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

    Self& operator++() {
        increment();
        return *this;
    }

    Self operator++(int) {
        Self tmp = *this;
        increment();
        return tmp;
    }

    Self& operator--() {
        decrement();
        return *this;
    }

    Self operator--(int) {
        Self tmp = *this;
        decrement();
        return tmp;
    }

    bool operator==(const Self& x) {
        return node_ == x.node_;
    }

    bool operator!=(const Self& x) {
        return node_ != x.node_;
    }
};

template<typename Key, typename Val,
        typename KeyOfValue, typename Compare, typename Alloc=std::allocator<Val>>
class AVLTree {
public:
    typedef Key key_type;
    typedef Val value_type;
    typedef Val* pointer;
    typedef Val& reference;
    typedef const Val* const_pointer;
    typedef const Val& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef AVLNode<Val>* NodePtr;
    typedef AVLNode<Val> Node;
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

    NodePtr createNode(const Val& x) {
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
    NodePtr& root() const { return header_->parent_; }

    NodePtr& leftmost() const { return header_->left_; }

    NodePtr& rightmost() const { return header_->right_; }

    static NodePtr& left(NodePtr x) { return x->left_; }

    static NodePtr& right(NodePtr x) { return x->right_; }

    static NodePtr& parent(NodePtr x) { return x->parent_; }

    static reference value(NodePtr x) { return x->value_; }

    static const Key& key(NodePtr x) { // need to return a const
        return KeyOfValue()(value(x));
    }

    typedef AVLTree<Key, Val, KeyOfValue, Compare, Alloc> Self;

    void init() {
        header_ = getNode();
        header_->balFactor_ = -2;
        root() = nullptr;
        leftmost() = header_;    // point to head itself
        rightmost() = header_;
        nodeCount_ = 0;
    }

public:
    typedef AVLTreeIterator<Val, Val&, Val*> iterator;
    typedef AVLTreeIterator<Val, const Val&, const Val*> const_iterator;

    AVLTree() { init(); }

    AVLTree(const Compare& comp) : keyComp_(comp) { init(); }

    iterator begin() { return iterator(leftmost()); }

    const_iterator begin() const { return const_iterator(leftmost()); }

    iterator end() { return iterator(header_); }

    const_iterator end() const { return const_iterator(header_); }

    bool empty() const { return nodeCount_ == 0; }

    size_type size() const { return nodeCount_; }

    size_type max_size() const { return NodeAllocTraits::max_size(nodeAlloc); }

    void clear() {
        _erase(root());
        leftmost() = header_;
        rightmost() = header_;
        root() = nullptr;
        nodeCount_ = 0;
    }

public:

    pair<iterator, bool> insertUnique(const Val& v) {
        NodePtr x = root();
        NodePtr y = header_;
        bool comp = true;
        while (x != nullptr) {
            y = x;
            comp = keyComp_(KeyOfValue()(v), key(x));
            x = comp ? left(x) : right(x);
        }
        iterator j = iterator(y);
        if (comp) { // left
            if (j == begin()) { // leftmost
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

    iterator insertEqual(const Val& v) {
        NodePtr x = root();
        NodePtr y = header_;
        while (x != nullptr) {
            y = x;
            x = keyComp_(KeyOfValue()(v), key(x)) ? left(x) : right(x);
        }
        return _insert(x, y, v);
    }

    iterator lowerBound(const Key& k) {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            if (!keyComp_(key(x), k)) {
                y = x;
                x = left(x);
            } else {
                x = right(x);
            }
        }
        return iterator(y);
    }

    const_iterator lowerBound(const Key& k) const {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            if (!keyComp_(key(x), k)) {
                y = x;
                x = left(x);
            } else {
                x = right(x);
            }
        }
        return const_iterator(y);
    }

    iterator upperBound(const Key& k) {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            if (keyComp_(k, key(x))) {
                y = x;
                x = left(x);
            } else {
                x = right(x);
            }
        }
        return iterator(y);
    }

    const_iterator upperBound(const Key& k) const {
        NodePtr y = header_;
        NodePtr x = root();
        while (x != nullptr) {
            if (keyComp_(k, key(x))) {
                y = x;
                x = left(x);
            } else {
                x = right(x);
            }
        }
        return const_iterator(y);
    }

    pair<iterator, iterator> equalRange(const Key& k) {
        return pair<iterator, iterator>(lowerBound(k), upperBound(k));
    }

    pair<const_iterator, const_iterator> equalRange(const Key& k) const {
        return pair<const_iterator, const_iterator>(lowerBound(k), upperBound(k));
    }

    void erase(iterator pos) {
        NodePtr y = _eraseAndRebalance(pos.node_);
        destroyNode(y);
        --nodeCount_;
    }

    void erase(iterator first, iterator last) {
        if (first == begin() && last == end()) {
            clear();
        } else {
            while (first != last) {
                erase(first++);
            }
        }
    }

    size_type erase(const Key& k) {
        pair<iterator, iterator> p = equalRange(k);
        size_type old = size();
        erase(p.first, p.second);
        return old - size();
    }

    iterator find(const Key& k) {
        iterator j = lowerBound(k);
        // 1. j will be end() if cannot find
        // 2. key(j) probably less than k
        return (j == end() || keyComp_(k, key(j.node_))) ? end() : j;
    }

    const_iterator find(const Key& k) const {
        const_iterator j = lowerBound(k);
        // 1. j will be end() if cannot find
        // 2. key(j) probably less than k
        return (j == end() || keyComp_(k, key(j.node_))) ? end() : j;
    }

    size_type count(const Key& k) const {
        pair<iterator, iterator> p = equalRange(k);
        size_type n = std::distance(p.first, p.second);
        return n;
    }

    bool operator==(Self& x) {
        return size() == x.size() && std::equal(begin(), end(), x.begin(), x.end());
    }

    bool operator<(Self& x) {
        return std::lexicographical_compare(begin(), end(), x.begin(), x.end());
    }

    bool operator!=(Self& x) {
        return !(operator==(x));
    }

    bool operator>(Self& x) {
        return x < *this;
    }

    bool operator<=(Self& x) {
        return !(operator>(x));
    }

    bool operator>=(Self& x) {
        return !(operator<(x));
    }

private:

    // erase all node in postorder
    void _erase(NodePtr x) {
        while (x != nullptr) {
            _erase(right(x));
            NodePtr y = left(x);
            destroyNode(x);
            x = y;
        }
    }

    // [x] is the child of [p],
    //  always is nullptr passed from insertUnique() and insertEqual(),
    //  do not need to consider, in theory.
    // [p] is the parent of [x]
    iterator _insert(NodePtr x, NodePtr p, const Val& v) {
        bool insertLeft = (x != nullptr ||
                           p == header_ || keyComp_(KeyOfValue()(v), key(p)));
        NodePtr z = createNode(v);
        _insertAndRebalance(insertLeft, z, p);
        ++nodeCount_;
        return iterator(z);
    }

    /*
     *       /                /
     *      x                y
     *     / \              / \
     *    a   y      =>    x   c
     *       / \          / \
     *     [b]   c       a  [b]
     */
    // rotate left
    // When the right sub-tree is taller, we need to rotate left to shorten it.
    void _rotateLeft(NodePtr x, NodePtr& root) {
        NodePtr y = x->right_;

        x->right_ = y->left_;
        y->left_ = x;

        y->parent_ = x->parent_;
        x->parent_ = y;

        if (x->right_ != nullptr) { // b
            x->right_->parent_ = x;
        }

        if (x == root) {
            root = y;
        } else if (y->parent_->right_ == x) { // x is the right son of his parent
            y->parent_->right_ = y;
        } else {
            y->parent_->left_ = y;
        }

        if (y->balFactor_ == 1) {
            /*
             *       /                /
             *      x                y
             *     / \              / \
             *    a   y      =>    x   c
             *       / \          / \ /
             *      b   c        a  b d
             *         /
             *        d
             */
            y->balFactor_ = 0;
            x->balFactor_ = 0;
        } else {
            /*
             *       /                /
             *      x                y
             *     / \              / \
             *    a   y      =>    x   c
             *       / \          / \
             *      b   c        a   b
             *       \               \
             *        d               d
             */
            y->balFactor_ = -1;
            x->balFactor_ = 1;
        }
    }

    /*
     *      /                /
     *     x                y
     *    / \      =>      / \
     *   y   a            c   x
     *  / \              / \
     * c  [b]          [b]  a
     */
    // rotate right
    // When the left sub-tree is taller, we need to rotate right to shorten it.
    void _rotateRight(NodePtr x, NodePtr& root) {
        NodePtr y = x->left_;

        x->left_ = y->right_;
        y->right_ = x;

        y->parent_ = x->parent_;
        x->parent_ = y;
        if (x->left_ != nullptr) { // b
            x->left_->parent_ = x;
        }

        if (x == root) {
            root = y;
        } else if (y->parent_->right_ == x) {
            y->parent_->right_ = y;
        } else {
            y->parent_->left_ = y;
        }

        if (y->balFactor_ == -1) {
            y->balFactor_ = 0;
            x->balFactor_ = 0;
        } else {
            y->balFactor_ = 1;
            x->balFactor_ = -1;
        }
    }

    /*
     *          /                      /
     *         a                      c
     *        / \                    / \
     *       b   g      =>          /   \
     *      / \                    b    a
     *     d   c                  / \  / \
     *        / \                d  e  f  g
     *       e   f
     */
    // rotate left and right.
    // As for Double rotation, we need to look four layers of tree.
    // When the top node of tree or sub-tree has 2 layers taller left sub-tree,
    //  and the left sub-tree have a taller right sub-tree,
    //  we need to do a left rotation
    //  to make the latter right sub-tree shorter firstly,
    //  and then do a right rotation
    //  to make the former left sub-tree shorter shorter secondly.
    void _rotateLeftRight(NodePtr a, NodePtr& root) {
        NodePtr b = a->left_;
        NodePtr c = b->right_;

        // a and b link to c' sons
        a->left_ = c->right_;
        b->right_ = c->left_;

        // c becomes the parent of a and b
        c->right_ = a;
        c->left_ = b;

        // c links to the parent of a and b
        c->parent_ = a->parent_;
        a->parent_ = b->parent_ = c;

        // check c' sons and link to their new parent
        if (a->left_) {  // f
            a->left_->parent_ = a;
        }
        if (b->right_) { // e
            b->right_->parent_ = b;
        }

        // the parent of a and b link to c
        if (a == root) {
            root = c;
        } else if (c->parent_->left_ == a) {
            c->parent_->left_ = c;
        } else {
            c->parent_->right_ = c;
        }

        switch (c->balFactor_) {
            case -1:
                /*
                 *    c
                 *   /
                 *  e
                 */
                a->balFactor_ = 1;
                b->balFactor_ = 0;
                break;
            case 0:
                a->balFactor_ = 0;
                b->balFactor_ = 0;
                break;
            case 1:
                /*
                *    c
                *     \
                *      f
                */
                a->balFactor_ = 0;
                b->balFactor_ = -1;
                break;
            default:
                assert(false);
        }
        c->balFactor_ = 0;
    }

    /*
     *       /                       /
     *       a                       c
     *      / \                     / \
     *     d   b      =>           /   \
     *        / \                 a     b
     *       c   g               / \   / \
     *      / \                 d   e  f  g
     *     e   f
     */
    // rotate right and left.
    // As for Double rotation, we need to look four layers of tree.
    // When the top node of tree or sub-tree has 2 layers taller right sub-tree,
    //  and the right sub-tree have a taller left sub-tree,
    //  we need to do a right rotation
    //  to make the latter left sub-tree shorter firstly,
    //  and then do a left rotation
    //  to make the former right sub-tree shorter shorter secondly.
    void _rotateRightLeft(NodePtr a, NodePtr& root) {
        NodePtr b = a->right_;
        NodePtr c = b->left_;

        a->right_ = c->left_;
        b->left_ = c->right_;

        c->left_ = a;
        c->right_ = b;

        c->parent_ = a->parent_;
        a->parent_ = b->parent_ = c;

        if (a->right_) {
            a->right_->parent_ = a;
        }
        if (b->left_) {
            b->left_->parent_ = b;
        }

        if (a == root) {
            root = c;
        } else if (c->parent_->left_ == a) {
            c->parent_->left_ = c;
        } else {
            c->parent_->right_ = c;
        }

        switch (c->balFactor_) {
            case -1:
                /*
                 *    c
                 *   /
                 *  e
                 */
                a->balFactor_ = 0;
                b->balFactor_ = 1;
                break;
            case 0:
                a->balFactor_ = 0;
                b->balFactor_ = 0;
                break;
            case 1:
                /*
                 *   c
                 *    \
                 *     f
                 */
                a->balFactor_ = -1;
                b->balFactor_ = 0;
                break;
            default:
                assert(false);
        }
        c->balFactor_ = 0;
    }

    // [x] is the position to insert
    // [p] is the parent of [x]
    void _insertAndRebalance(bool insertLeft, NodePtr x, NodePtr p) {

        // construct the new node to insert
        x->parent_ = p;
        x->left_ = nullptr;
        x->right_ = nullptr;
        x->balFactor_ = 0;

        if (insertLeft) {
            p->left_ = x;
            if (p == header_) { // consider that p is header
                header_->parent_ = x;
                header_->right_ = x;
            } else if (p == header_->left_) { // consider that p is son of header
                header_->left_ = x;
            }
        } else {
            p->right_ = x;
            if (p == header_->right_) {
                header_->right_ = x;
            }
        }

        // rebalance
        while (x != root()) {
            switch (x->parent_->balFactor_) {
                case 0:
                    // same tall right and left sub tree of x parent.
                    // One of them will become taller after insert a node.
                    x->parent_->balFactor_ = (x == x->parent_->left_) ? -1 : 1;
                    x = x->parent_; // percolate up the path
                    break;
                case 1:
                    // right sub-tree is taller
                    if (x == x->parent_->left_) {
                        // If inserted node in the left, become same tall.
                        x->parent_->balFactor_ = 0;
                    } else {
                        // If inserted node in the right, become more taller probably.
                        // For shortening it, we need to rotate it.
                        if (x->balFactor_ == -1) {
                            // x have a taller left, do a right left rotation to shorten the left
                            _rotateRightLeft(x->parent_, root());
                        } else {
                            // just do a left rotation to shorten the right.
                            _rotateLeft(x->parent_, root());
                        }
                    }
                    // adjust once is enough
                    return;
                case -1:
                    // -1 mean that x parent have a taller sub-left tree
                    if (x == x->parent_->left_) {
                        // but insert a new node in sub-left tree
                        //  leading to become more taller (probably)
                        // For shortening it, we need to rotate it.
                        if (x->balFactor_ == 1) {
                            // If x have a taller sub-right tree,
                            //  doing a left right rotation can shorten the right.
                            _rotateLeftRight(x->parent_, root());
                        } else {
                            // Else, just do a right rotation to shorten the left.
                            _rotateRight(x->parent_, root());
                        }
                    } else {
                        x->parent_->balFactor_ = 0;
                    }
                    return;
                default:
                    assert(false);
            }
        }
    }

    // erase node [z] and return the deleted node
    NodePtr _eraseAndRebalance(NodePtr z) {
        NodePtr y = z;
        NodePtr x = nullptr;
        NodePtr xParent = nullptr;

        // x is the child of y
        if (y->left_ == nullptr) {
            x = y->right_;
        } else if (y->right_ == nullptr) {
            x = y->left_;
        } else {
            // y has two children
            // find the successor
            y = y->right_;
            while (y->left_) {
                y = y->left_;
            }
            x = y->right_; //y has no left child, so x is the successor
        }

        if (y != z) { // mean that z has two children
            // we make the successor y to replace z
            // z will disconnect to its children and y link to them
            z->left_->parent_ = y;
            y->left_ = z->left_;
            if (y != z->right_) {
                // y will be moved, need to link its child to its parent
                xParent = y->parent_;
                if (x) {
                    x->parent_ = y->parent_;
                }
                y->parent_->left_ = x;
                y->right_ = z->right_;
                z->right_->parent_ = y;
            } else {
                // y == z->right, mean y has no left child and x is the right of y
                xParent = y;
            }
            // link z's parent to y
            if (root() == z) {
                root() = y;
            } else if (z->parent_->left_ == z) {
                z->parent_->left_ = y;
            } else {
                z->parent_->right_ = y;
            }
            y->parent_ = z->parent_;
            y->balFactor_ = z->balFactor_;

        } else { // mean that z has one child or none and y == z
            // If z has one child, we just make its child as the successor
            // x become the successor
            xParent = y->parent_;
            if (x) {
                x->parent_ = y->parent_;
            }
            if (root() == z) {
                root() = x;
            } else if (z->parent_->left_ == z) {
                z->parent_->left_ = x;
            } else {
                z->parent_->right_ = x;
            }
            if (leftmost() == z) {
                if (z->right_ == nullptr) {
                    leftmost() = z->parent_;
                } else {
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

        // rebalance
        while (x != root()) {
            switch (xParent->balFactor_) {
                case 0:
                    xParent->balFactor_ = (x == xParent->right_) ? -1 : 1;
                    return z;
                case -1:
                    // left sub-tree taller
                    if (x == xParent->left_) { // erase the node in the left, balance just right
                        xParent->balFactor_ = 0;
                        x = xParent;
                        xParent = xParent->parent_;
                    } else { // erase the node in the right, need to shorten the left
                        NodePtr a = xParent->left_;
                        if (a->balFactor_ == 1) { // check whether need a double rotation
                            _rotateLeftRight(a, root());
                        } else {
                            _rotateRight(xParent, root());
                        }
                        // percolate up
                        x = xParent->parent_;
                        xParent = xParent->parent_->parent_;
                        if (x->balFactor_ == 1) {
                            return z;
                        }
                    }
                    break;
                case 1:
                    // right sub-tree taller
                    if (x == xParent->right_) {
                        xParent->balFactor_ = 0;
                        x = xParent;
                        xParent = xParent->parent_;
                    } else {
                        NodePtr a = xParent->right_;
                        if (a->balFactor_ == -1) {
                            _rotateRightLeft(xParent, root());
                        } else {
                            _rotateLeft(xParent, root());
                        }
                        x = xParent->parent_;
                        xParent = xParent->parent_->parent_;
                        if (x->balFactor_ == -1) {
                            return z;
                        }
                    }
                    break;
                default:
                    assert(false);
            }
        }
        return z;
    }
};

template<typename Val>
int height(const AVLNode<Val> root) {
    int result = 0;
    if (root) {
        int hleft = height(root.left_);
        if (hleft < 0) {
            return hleft;
        }
        int hright = height(root.right_);
        if (hright < 0) {
            return hright;
        }
        result = std::max(hleft, hright) + 1;
        int balance = hright - hleft;
        if (balance < -1 || balance > 1) {
            return -1;
        }
        if (balance != root.balFactor_) {
            return -2;
        }
    }
    return result;
}


template<typename Val>
int checkBalance(const AVLNode<Val> root) {
    return height(root) >= 0;
}

}

#endif //ALG_AVLTREE_H
