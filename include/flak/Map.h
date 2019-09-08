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

#ifndef ALG_MAP_H
#define ALG_MAP_H

#include <functional>
#include "RBTree.h"

using std::pair;

namespace flak {

template<class Key, class T,
        class Compare = std::less<Key>,
        class Alloc = std::allocator<pair<const Key, T>>>
class Map {
public:
    typedef Key key_value;
    typedef T data_type;
    typedef T mapped_type;
    typedef pair<const Key, T> value_type;  // value_type is pair
    typedef Compare key_compare;

    class value_compare {
    protected:
        Compare comp_;

        explicit value_compare(Compare c) : comp_(c) {}

    public:
        bool operator()(const value_type &x, const value_type &y) const {
            return comp_(x.first, y.first);
        }
    };

    // rebind the allocator
    typedef typename Alloc::template rebind<value_type>::other PairValueAlloc;
    typedef std::allocator_traits<PairValueAlloc> PairValueAllocTraits;

    typedef Map<Key, T, Compare, PairValueAlloc> Self;

private:
    typedef RBTree<key_value, value_type, std::_Select1st<value_type>,
            key_compare, PairValueAlloc> rep_type;

    rep_type t_;
public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;

    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_typ;

    Map() : t_(Compare()) {}

    explicit Map(const Compare &comp) : t_(comp) {}

    template<class InputIterator>
    Map(InputIterator first, InputIterator last) : t_(Compare()) {
        t_.insertUnique(first, last);
    }

    template<class InputIterator>
    Map(InputIterator first, InputIterator last, const Compare &comp): t_(Compare()) {
        t_.insertUnique(first, last);
    }

    Self &operator=(const Self &x) {
        t_ = x.t_;
        return *this;
    }

    key_compare key_comp() const { return t_.key_comp(); }

    Compare value_comp() const { return value_compare(t_.key_comp()); }

    iterator begin() { return t_.begin(); }

    const_iterator begin() const { return t_.begin(); };

    iterator end() { return t_.end(); }

    const_iterator end() const { return t_.end(); }

    bool empty() const { return t_.empty(); }

    size_type size() const { return t_.size(); }

    size_type max_size() const { return t_.max_size(); }

    T &operator[](const Key &k) {
        return (*(insert(value_type(k, T())).first)).second;
    }

    void swap(Self &x) { t_.swap(x.t_); }

    pair<iterator, bool> insert(const value_type &x) {
        return t_.insertUnique(x);
    }

    iterator insert(iterator pos, const value_type &x) {
        return t_.insertUnique(pos, x);
    }

    template<class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        t_.insertUnique(first, last);
    }

    void erase(iterator pos) { t_.erase(pos); }

    size_type erase(const Key &k) { return t_.erase(k); }

    void erase(iterator first, iterator last) { t_.erase(first, last); }

    void clear() { t_.clear(); }

    iterator find(const Key &k) { return t_.find(k); }

    const_iterator find(const Key &k) const { return t_.find(k); }

    size_type count(const Key &k) const { return t_.count(k); }

    iterator lowerBound(const Key &k) { return t_.lowerBound(k); }

    const_iterator lowerBound(const Key &k) const { return t_.lowerBound(k); }

    iterator upperBound(const Key &k) { return t_.upperBound(k); }

    const_iterator upperBound(const Key &k) const { return t_.upperBound(k); }

    pair<iterator, iterator> equalRange(const Key &x) {
        return t_.equalRange(x);
    }

    pair<const_iterator, const_iterator> equalRange(const Key &x) const {
        return t_.equalRange(x);
    }

    bool operator==(const Self &x) {
        return t_ == x.t_;
    }

    bool operator<(const Self &x) {
        return t_ < x.t_;
    }
};

}
#endif //ALG_MAP_H
