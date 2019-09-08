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

#ifndef ALG_SLIST_H
#define ALG_SLIST_H

#include <iterator>

namespace flak {

template<class T>
struct SListNode {
    SListNode<T> *next_;
    T data_;
};

// link [nowNode] to the after of [prevNode]
template<class T>
inline SListNode<T> *_makeLink(SListNode<T> *prevNode, SListNode<T> *newNode) {
    newNode->next_ = prevNode->next_;
    prevNode->next_ = newNode;
    return newNode;
}

// get size of slist
template<class T>
inline size_t _slistSize(SListNode<T> *node) {
    size_t res = 0;
    for (; node != nullptr; node = node->next_) {
        ++res;
    }
    return res;
}

// find the previous of [x]
template<class T>
inline SListNode<T> *_slistPrevious(SListNode<T> *head, SListNode<T> *x) {
    while (head && head->next_ != x) {
        head = head->next_;
    }
    return head;
}

// reverse the slist started with [cur]
template<class T>
inline SListNode<T> *_slistReverse(SListNode<T> *cur) {
    SListNode<T> *prev = cur;
    cur = cur->next_;
    prev->next_ = nullptr;   // the first will become the last, so need to link to the nullptr
    while (cur) {
        SListNode<T> *next = cur->next_;
        cur->next_ = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}


template<class T, class Ref, class Ptr>
struct SListIterator {
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    typedef SListNode<T> *NodePtr;
    typedef SListNode<T> Node;
    typedef SListIterator<T, Ref, Ptr> Self;

    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;

    NodePtr node_;

    explicit SListIterator(NodePtr x) : node_(x) {}

    SListIterator() {}

    void incr() { node_ = node_->next_; }

    bool operator==(const Self &x) const {
        return node_ == x.node_;
    }

    bool operator!=(const Self &x) const {
        return node_ != x.node_;
    }

    reference operator*() const { return node_->data_; }

    pointer operator->() const { return &(operator*()); }

    Self &operator++() {
        incr();
        return *this;
    }

    Self operator++(int) {
        Self tmp = *this;
        incr();
        return tmp;
    }
};

template<class T, class Alloc = std::allocator<T>>
class SList {
public:
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef const T *const_pointer;
    typedef const T &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef SListIterator<T, T &, T *> iterator;
    typedef SListIterator<T, const T &, const T *> const_iterator;

protected:
    typedef SList<T, Alloc> Self;
    typedef SListNode<T> Node;
    typedef SListNode<T> *NodePtr;
    typedef typename Alloc::template rebind<T>::other ValueAlloc;
    typedef std::allocator_traits<ValueAlloc> ValueAllocTraits;
    typedef typename ValueAllocTraits::template rebind_alloc<SListNode<T>> NodeAlloc;
    typedef std::allocator_traits<NodeAlloc> NodeAllocTraits;

    NodeAlloc nalloc;

    NodePtr createNode(const T &x) {
        NodePtr node = NodeAllocTraits::allocate(nalloc, x);
        NodeAllocTraits::construct(nalloc, &(node->data_), x);
        node->next_ = nullptr;
        return node;
    }

    void destroyNode(NodePtr p) {
        NodeAllocTraits::destroy(nalloc, &(p->data_));
        NodeAllocTraits::destroy(nalloc, p);
        NodeAllocTraits::deallocate(nalloc, p, 1);
    }

private:
    NodePtr head_;

public:
    SList() {
        head_ = createNode(T());
        head_->next_ = nullptr;
    }

    ~SList() { clear(); }

    iterator begin() const { return iterator(head_->next_); }

    iterator end() const { return iterator(nullptr); }

    size_type size() const { return _slistSize(head_->next_); }

    bool empty() const { return head_->next_ == nullptr; }

    void swap(Self &sl) {
        NodePtr tmp = head_->next_;
        head_->next_ = sl.head_->next_;
        sl.head_->next_ = tmp;
    }

    void clear() {
        _erase_after(head_, nullptr);
    }

public:
    reference front() const { return head_->next_->data_; }

    void push_front(const T &x) {
        _makeLink(head_, createNode(x));
    }

    void pop_front() {
        NodePtr x = head_->next_;
        head_->next_ = x->next_;
        destroyNode(x);
    }

private:
    template<class InputIter>
    void _insert_after_range(NodePtr pos, InputIter first, InputIter last) {
        while (first != last) {
            pos = _makeLink(pos, createNode(*first));
            ++first;
        }
    }

public:
    SList(std::initializer_list<T> ls) : SList() {
        _insert_after_range(head_, ls.begin(), ls.end());
    }

    template<class InputIter>
    SList(InputIter first, InputIter last) : SList() {
        _insert_after_range(head_, first, last);
    }

private:
    NodePtr previous(NodePtr pos) const {
        return _slistPrevious(head_, pos);
    }

public:
    // find the previous of [pos]. O(n)
    iterator previous(iterator pos) const {
        return iterator(_slistPrevious(head_, pos.node_));
    }

private:
    NodePtr _insert_after(NodePtr pos, const T &x) {
        return _makeLink(pos, createNode(x));
    }

    NodePtr _insert_after(iterator pos, const T &x) {
        return _makeLink(pos.node_, createNode(x));
    }

public:
    // insert x into the after of [pos], return the node of x
    iterator insert_after(iterator pos, const T &x) {
        return iterator(_insert_after(pos, x));
    }

    // insert x into the previous of [pos], return the node of x
    // O(n)
    iterator insert(iterator pos, const T &x) {
        return iterator(_insert_after(previous(pos.node_), x));
    }

private:
    NodePtr _erase_after(NodePtr x) {
        NodePtr next = x->next_;
        NodePtr nextNext = next->next_;
        destroyNode(next);
        x->next_ = nextNext;
        return nextNext;
    }

    NodePtr _erase_after(NodePtr beforeFirst, NodePtr last) {
        NodePtr cur = beforeFirst->next_;
        while (cur != last) {
            NodePtr tmp = cur;
            cur = cur->next_;
            destroyNode(tmp);
        }
        beforeFirst->next_ = last;
        return last;
    }

public:
    // erase the after of [pos] and return its next node
    iterator erase_after(iterator pos) {
        return iterator(_erase_after(pos.node_));
    }

    // erase the [pos] and return its next node
    iterator erase(iterator pos) {
        return iterator(_erase_after(previous(pos.node_)));
    }

    // erase [[beforeFirst] + 1, last) and return the last
    iterator erase_after(iterator beforeFirst, iterator last) {
        return iterator(_erase_after(beforeFirst, last));
    }

    // erase [first, last) and return the last
    iterator erase(iterator first, iterator last) {
        return iterator(_erase_after(previous(first.node_), last.node_));
    }

private:
    // move [beforeFirst + 1, beforeLast] to the after of pos
    void _splice_after(NodePtr pos, NodePtr beforeFirst, NodePtr beforeLast) {
        if (pos != beforeFirst && pos != beforeLast) {
            NodePtr first = beforeFirst->next_;
            beforeFirst->next_ = beforeLast->next_;
            beforeLast->next_ = pos->next_;
            pos->next_ = first;
        }
    }

    void _splice_after(iterator pos, iterator beforeFirst, iterator beforeLast) {
        _splice_after(pos.node_, beforeFirst.node_, beforeLast.node_);
    }

public:
    // move [beforeFirst + 1, beforeLast] to the after of pos
    void splice_after(iterator pos, iterator beforeFirst, iterator beforeLast) {
        if (beforeFirst != beforeLast) {
            _splice_after(pos, beforeFirst, beforeLast);
        }
    }

    // move [prevOfx] + 1 to the after of pos
    void splice_after(iterator pos, iterator prevOfx) {
        _splice_after(pos.node_, prevOfx.node_, prevOfx.node_->next_);
    }

    // move slist [x] to the after of [pos]
    void splice_after(iterator pos, Self &x) {
        _splice_after(pos.node_,
                      x.head_,
                      _slistPrevious(head_, NodePtr(nullptr)));
    }

    // move slist [x] to the previous of [pos]
    void splice(iterator pos, Self &x) {
        _splice_after(previous(pos.node_),
                      x.head_,
                      _slistPrevious(x.head_, NodePtr(nullptr)));
    }

    // move [xpos] of x to the previous of [pos]
    void splice(iterator pos, Self &x, iterator xpos) {
        _splice_after(previous(pos.node_),
                      _slistPrevious(x.head_, xpos.node_),
                      xpos.node_);
    }

    // move [first, last) to the previous of [pos]
    void splice(iterator pos, Self &x, iterator first, iterator last) {
        if (first != last) {
            _splice_after(previous(pos.node_),
                          _slistPrevious(x.head_, first.node_),
                          _slistPrevious(first.node_, last.node_));
        }

    }

public:
    // reverse the slist
    void reverse() {
        if (head_->next_) {
            head_ = _slistReverse(head_->next_);
        }
    }

    // merge two sorted slists in ascending
    void merge(Self &x) {
        NodePtr cur = head_;
        while (cur->next_ && x.head_->next_) {
            if (x.head_->next_->data_ < cur->next_->data_) {
                // move [x.head.next] to the after of [cur], the previous of [cur.next]
                _splice_after(cur, x.head_, x.head_->next_);
            }
            cur = cur->next_;
        }
        // the all rest nodes of x link to cur last
        if (x.head_->next_) {
            cur->next_ = x.head_->next_;
            x.head_->next_ = nullptr;
        }
    }

    // non-recursion merge sort.
    // counter is the buffer to save elements. The i-th coutner contains 2^i element.
    void sort() {
        // empty or size of 1
        if(head_->next_ == nullptr || head_->next_->next_ == nullptr) {
            return;
        }
        Self carry;
        Self counter[64];
        int fill = 0;
        while (!empty()) {
            _splice_after(carry.head_, head_, head_->next_);
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if(i == fill) {
                ++fill;
            }
        }
        for(int i = 1; i < fill; ++i) {
            counter[i].merge(counter[i-1]);
        }
        swap(counter[fill-1]);
    }

};

}

#endif //ALG_SLIST_H
