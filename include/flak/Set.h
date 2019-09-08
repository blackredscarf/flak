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


#ifndef ALG_SET_H
#define ALG_SET_H

#include <functional>
#include "RBTree.h"
using std::pair;

namespace flak {

template<class Key, class Compare = std::less<Key>, class Alloc = std::allocator<Key>>
class Set {
    typedef Key key_type;
    typedef Key value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
private:

    typedef RBTree<key_type, value_type,
            std::_Identity<value_type>, key_compare, Alloc> rep_type;

    // define a rbtree object
    rep_type t_;
public:
    // all types are const
    typedef typename rep_type::const_pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::const_reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::const_iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    typedef Set<Key, Compare, Alloc> Self;

    Set() : t_(Compare()) {}

    template<class InputIterator>
    Set(InputIterator first, InputIterator last)
            : t_(Compare()) {
        t_.insertUnique(first, last);
    }

    Set(const Self &x) : t_(x.t_) {}

    Self &operator=(const Self &x) {
        t_ = x.t;
        return *this;
    }

    key_compare key_comp() const { return t_.key_comp(); }

    Alloc get_allocator() const { return t_.get_allocator(); }

    iterator begin() const { return t_.begin(); }

    iterator end() const { return t_.end(); }

    bool empty() const { return t_.empty(); }

    size_type size() const { return t_.size(); }

    size_type max_size() const { return t_.max_size(); }

    void swap(Self &x) { t_.swap(x.t_); }

    void clear() { t_.clear(); }

    // insert
    pair<iterator, bool> insert(const value_type &x) {
        pair<typename rep_type::iterator, bool> p = t_.insertUnique(x);
        return pair<iterator, bool>((iterator &) p.first, p.second);
    }

    iterator insert(iterator pos, const value_type &x) {
        return t_.insertUnique(pos, x);
    }

    void insert(const_iterator first, const_iterator last) {
        t_.insertUnique(first, last);
    }

    void insert(pointer first, pointer last) {
        t_.insertUnique(first, last);
    }

    // erase
    void erase(iterator pos) {
        typedef typename rep_type::iterator non_const_iter;
        t_.erase((non_const_iter &) pos);
    }

    size_type erase(const key_type &x) {
        return t_.erase(x);
    }

    void erase(iterator first, iterator last) {
        typedef typename rep_type::iterator non_const_iter;
        t_.erase((non_const_iter &) first, (non_const_iter &) last);
    }

    // find
    iterator find(const key_type &x) const { return t_.find(x); }

    size_type count(const key_type &x) const {
        return t_.find(x) == t_.end() ? 0 : 1;
    }

    iterator lowerBound(const key_type &x) const {
        return t_.lowerBound(x);
    }

    iterator upperBound(const key_type &x) const {
        return t_.upperBound(x);
    }

    bool operator==(const Self &x) {
        return t_ == x.t_;
    }

    bool operator<(const Self &x) {
        return t_ < x.t_;
    }
};

}
#endif //ALG_SET_H
