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

#ifndef ALG_MY_LIST_H
#define ALG_MY_LIST_H

#include <cstddef>
#include <memory>
#include <iterator>
using std::bidirectional_iterator_tag;
using std::ostream;

namespace flak {

template<class T>
struct ListNode {
    ListNode<T> *prev_;
    ListNode<T> *next_;
    T data_;

    ListNode(T &data) :
            data_(data), prev_(nullptr), next_(nullptr) {}
};

template<class T, class Ref, class Ptr>
struct ListIter {
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef ptrdiff_t difference_type;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ListIter<T, Ref, Ptr> self;

    ListNode<T> *node_;

    ListIter() {}

    explicit ListIter(ListNode<T> *node) : node_(node) {}

    reference operator*() const { return node_->data_; }

    pointer operator->() const { return &(node_->data_); }

    self &operator++() {
        node_ = node_->next_;
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        operator++();
        return tmp;
    }

    self &operator--() {
        node_ = node_->prev_;
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        operator--();
        return tmp;
    }

    bool operator==(const self x) const {
        return node_ == x.node_;
    }

    bool operator!=(const self x) const {
        return node_ != x.node_;
    }
};

template<class T, class Alloc = std::allocator<T>>
class List {
public:
    typedef ListIter<T, T &, T *> iterator;
    typedef ListIter<T, const T &, const T *> const_iterator;

    // rebind the value allocator
    typedef typename Alloc::template rebind<T>::other ValueAlloc;
    typedef std::allocator_traits<ValueAlloc> ValueAllocTraits;
    // rebind the node allocator
    typedef typename ValueAllocTraits::template rebind_alloc<ListNode<T>> NodeAlloc;
    typedef std::allocator_traits<NodeAlloc> NodeAllocTraits;

    typedef ListNode<T> Node;
    typedef List<T, Alloc> Self;

protected:
    NodeAlloc nalloc;

    // allocate
    Node *getNode() {
        return NodeAllocTraits::allocate(nalloc, 1);
    }

    // construct
    Node *createNode(const T &x) {
        Node *p = getNode();
        NodeAllocTraits::construct(nalloc, &(p->data_), x);
        return p;
    }

    // deallocate
    void putNode(Node *p) {
        NodeAllocTraits::deallocate(nalloc, p, 1);
    }

    // destroy
    void destroyNode(Node *p) {
        NodeAllocTraits::destroy(nalloc, &(p->data_));
        NodeAllocTraits::destroy(nalloc, p);
    }

public:
    List() {
        node_ = getNode();
        size_ = 0;
        node_->prev_ = node_;   // link to itself
        node_->next_ = node_;
    }

    List(std::initializer_list<T> ls) : List() {
        for (auto v : ls) {
            push_back(v);
        }
    }

    iterator begin() { return iterator(node_->next_); }

    iterator end() { return iterator(node_); }

    const_iterator begin() const { return const_iterator(node_->next_); }

    const_iterator end() const { return const_iterator(node_); }

    bool empty() const { return node_->next_ == node_; }

    size_t size() const {
        return size_;
    }

    T &front() { return *begin(); }

    const T *front() const { return *begin(); }

    T &back() { return *(--end()); }

    const T *back() const { return *(--end()); }

    ~List() { clear(); }

public:
    iterator insert(iterator pos, const T &x) {
        Node *newNode = createNode(x);
        newNode->next_ = pos.node_;
        newNode->prev_ = pos.node_->prev_;
        pos.node_->prev_->next_ = newNode;
        pos.node_->prev_ = newNode;
        inc_size(1);
        return iterator(newNode);
    }

    iterator insert(iterator pos, const T *first, const T *last) {
        iterator curIter = end();
        for (; first != last; first++) {
            curIter = insert(pos, *first);
        }
        return curIter;
    }

    void push_back(const T &x) {
        insert(end(), x);
    }

    void push_front(const T &x) {
        insert(begin(), x);
    }

    // erase the element in [pos]
    iterator erase(iterator pos) {
        Node *next = pos.node_->next_;
        Node *prev = pos.node_->prev_;
        next->prev_ = prev;
        prev->next_ = next;
        destroyNode(pos.node_);
        putNode(pos.node_);
        dec_size(1);
        return iterator(next);
    }

    // erase element of the specific value
    void remove(const T &x) {
        iterator first = begin();
        iterator last = end();
        while (first != last) {
            iterator next = first;
            ++next;
            if (*first == x) {
                erase(first);
            }
            first = next;
        }
    }

    // pop the last element
    void pop_back() {
        iterator tmp = end();
        erase(--tmp);
    }

    // pop the first element
    void pop_front() {
        erase(begin());
    }

    void clear() {
        Node *curNode = node_->next_;
        while (curNode != node_) {
            Node *tmp = curNode;
            curNode = curNode->next_;
            destroyNode(tmp);
            putNode(tmp);
        }
        node_->next_ = node_;
        node_->prev_ = node_;
    }

public:
    // connect [_list] to the previous of [pos]
    void splice(iterator pos, List &_list) {
        if (!_list.empty()) {
            transfer(pos, _list.begin(), _list.end());
            inc_size(_list.size_);
            _list.dec_size(_list.size_);
        }
    }

    // connect the [posOflist] of [_list] to the previous of [pos]
    void splice(iterator pos, List &_list, iterator posOflist) {
        iterator j = posOflist;
        ++j;
        if(pos == posOflist || pos == j) {
            return;
        }
        transfer(pos, posOflist, j);
    }

    // connect  [first, last) of [_list] to the previous of [pos]
    void splice(iterator pos, List &_list, iterator first, iterator last) {
        if (first == last) {
            return;
        }
        if (this == &_list) {
            transfer(pos, first, last);
            return;
        } else {
            int n = std::distance(first, last);
            inc_size(n);
            _list.dec_size(n);
            transfer(pos, first, last);
        }
    }

    // reverse the list by transfer(begin(), cur_, cur_+1)
    void reverse() {
        // 长度为0或1时不需要翻转
        if (node_->next_ == node_ || node_->next_->next_ == node_) {
            return;
        }
        iterator first = begin();
        ++first;
        while (first != end()) {
            iterator old = first;
            ++first;
            transfer(begin(), old, first);
        }
    }

    // merge [x] to *this. Ascending order is required for [x] and *this.
    void merge(Self& x) {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();
        while(first1 != last1 && first2 != last2) {
            if(*first2 < *first1) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            } else {
                ++first1;
            }
        }
        if(first2 != last2) {
            transfer(last1, first2, last2);
        }
    }

    // swap two header nodes.
    void swap(Self& x) {
        std::swap(node_, x.node_);
    }

    // non-recursion merge sort.
    // counter is the buffer to save elements. The i-th coutner contains 2^i element.
    void sort() {
        // empty or size of 1
        if(node_->next_ == node_ || node_->next_->next_ == node_) {
            return;
        }
        Self carry;
        Self counter[64];
        int fill = 0;
        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());
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

public:
    bool operator==(List &_list) {
        if (size_ != _list.size()) {
            return false;
        }
        iterator e1 = end();
        iterator e2 = _list.end();
        iterator s1 = begin();
        iterator s2 = _list.begin();
        while (s1 != e1 && s2 != e2 && *s1 == *s2) {
            ++s1;
            ++s2;
        }
        return s1 == e1 && s2 == e2;
    }

    bool operator==(List &&x) {
        return operator==(x);
    }

    friend ostream &operator<<(ostream &os, const Self &x) {
        const_iterator first = x.begin();
        while (first != x.end()) {
            os << *first << " ";
            ++first;
        }
        return os;
    }

protected:
    // move [first, last) to the previous of pos
    void transfer(iterator pos, iterator first, iterator last) {
        if (pos != last) {
            last.node_->prev_->next_ = pos.node_;
            first.node_->prev_->next_ = last.node_;
            pos.node_->prev_->next_ = first.node_;
            auto *tmp = pos.node_->prev_;
            pos.node_->prev_ = last.node_->prev_;
            last.node_->prev_ = first.node_->prev_;
            first.node_->prev_ = tmp;
        }
    }

    void inc_size(int n) {
        size_ += n;
    }

    void dec_size(int n) {
        size_ -= n;
    }

private:
    ListNode<T> *node_;  // last element
    size_t size_;
};

}
#endif //ALG_MY_LIST_H
