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

*/#ifndef FLAK_HASHSET_H
#define FLAK_HASHSET_H

#include "HashTable.h"
#include <functional>
#include <utility>
namespace flak {

template<class Val,
        class HashFcn = std::hash<Val>,
        class EqualKey = std::equal_to<Val>,
        class Alloc = std::allocator<Val>>
class HashSet {
private:
    typedef HashTable<Val, Val, HashFcn, std::_Identity<Val>, EqualKey, Alloc> ht;
    ht rep;
public:
    typedef typename ht::key_type key_type;
    typedef typename ht::value_type value_type;
    typedef typename ht::hasher hasher;
    typedef typename ht::key_equal key_equal;

    typedef typename ht::size_type size_type;
    typedef typename ht::difference_type difference_type;
    typedef typename ht::const_pointer pointer;
    typedef typename ht::const_pointer const_pointer;
    typedef typename  ht::const_reference reference;
    typedef typename  ht::const_reference const_reference;
    typedef typename ht::const_iterator iterator;
    typedef typename ht::const_iterator const_iterator;

    hasher hashFunc() const { return rep.hashFunc(); }
    key_equal keyEq() const { return rep.keyEq(); }

protected:
    typedef HashSet<Val, HashFcn, EqualKey, Alloc> Self;

public:
    HashSet() : rep(100, hasher(), key_equal()) {}
    explicit HashSet(size_type n) : rep(n, hasher(), key_equal()) {}
    HashSet(size_type n, const hasher& hf) : rep(n, hf, key_equal()) {}
    HashSet(size_type n, const hasher& hf, const key_equal& eql)
        : rep(n, hf, eql) {}

    template<class InputIterator>
    HashSet(InputIterator first, InputIterator last)
        : rep(100, hasher(), key_equal()) { rep.insertUnique(first, last); }

    template<class InputIterator>
    HashSet(InputIterator first, InputIterator last, size_type n)
            : rep(n, hasher(), key_equal()) { rep.insertUnique(first, last); }

    template<class InputIterator>
    HashSet(InputIterator first, InputIterator last, size_type n, const hasher& hf)
            : rep(n, hf, key_equal()) { rep.insertUnique(first, last); }

    template<class InputIterator>
    HashSet(InputIterator first, InputIterator last,
            size_type n, const hasher& hf, const key_equal& eql)
            : rep(n, hf, eql) { rep.insertUnique(first, last); }

public:
    size_type size() const { return rep.size(); }
    size_type max_size() const { return rep.max_size(); }
    bool empty() const { return rep.empty(); }

    iterator begin() const { return rep.begin(); }
    iterator end() const { return rep.end(); }

public:
    pair<iterator, bool> insert(const value_type& obj) {
        auto p = rep.insertUnique(obj);
        return pair<iterator, bool>(
                (iterator&) p.first,
                p.second);
    }

    template<class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        rep.insertUnique(first, last);
    }

    void insert(const_iterator first, const_iterator last) {
        rep.insertUnique(first, last);
    }

    pair<iterator, bool> insertNoresize(const value_type& obj) {
        auto p = rep.insertUniqueNoresize(obj);
        return pair<iterator, bool>(
                (iterator&) p.first,
                p.second);
    }

    iterator find(const key_type& key) const { return rep.find(key); }
    size_type count(const key_type& key) const { return rep.count(key); }

    size_type erase(const key_type& key) { return rep.erase(key); }
    void erase(iterator it) { rep.erase(it); }
    void erase(iterator first, iterator last) { rep.erase(first, last); }
    void clear() { rep.clear(); }

public:
    void resize(size_type hint) { rep.resize(hint); }
    size_type bucketCount() const { return rep.bucketCount(); }
    size_type maxBucketCount() const { return rep.maxBucketCount(); }
    size_type elemsInBucket(size_type n) const { return rep.elemsInBucket(n); }

    bool operator==(const Self& ht) {
        return rep == ht.rep;
    }
};


}
#endif //FLAK_HASHSET_H
