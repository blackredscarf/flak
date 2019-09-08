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

// HashTable.
// You can learn it at https://en.wikipedia.org/wiki/Hash_table .
// the structure model like the below:
//
// buckets : [0, 1, 2, 3, 4, 5, 6, 7]   (vector)
//            |              |
//           |-> n1 -> n2   |-> n1 -> n2 -> n3 (list)


#ifndef ALG_HASHTABLE_H
#define ALG_HASHTABLE_H

#include <iterator>
#include <vector>
#include <algorithm>

using std::vector;
using std::lower_bound;
using std::pair;


namespace flak {

template<class T>
struct HashTableNode {
    HashTableNode* next_;
    T val_;
};

template<class Value, class Key, class HashFcn,
        class ExtractKey, class EqualKey, class Alloc = std::allocator<Value>>
class HashTable;

template<class Value, class Key, class HashFcn,
        class ExtractKey, class EqualKey, class Alloc>
struct HashTableIterator {
    typedef HashTable<Value, Key, HashFcn,
            ExtractKey, EqualKey, Alloc> HashTable;
    typedef HashTableIterator<Value, Key, HashFcn,
            ExtractKey, EqualKey, Alloc> iterator;
    typedef HashTableNode<Value> Node;
    typedef HashTableNode<Value>* NodePtr;

    typedef std::forward_iterator_tag iterator_category;
    typedef Value value_type;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef Value& reference;
    typedef Value* pointer;

    NodePtr cur_;
    HashTable* ht_;

    HashTableIterator(NodePtr n, HashTable* tab) : cur_(n), ht_(tab) {}

    HashTableIterator() = default;

    reference operator*() const { return cur_->val_; }

    pointer operator->() const { return &(cur_->val_); }

    iterator& operator++() {
        const NodePtr old = cur_;
        cur_ = cur_->next_;
        if (!cur_) {
            size_type bucket = ht_->bkt_num(old->val_);
            while (!cur_ && ++bucket < ht_->buckets_.size()) {
                cur_ = ht_->buckets_[bucket];
            }
        }
        return *this;
    }

    iterator operator++(int) {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

    bool operator==(const iterator& it) const { return cur_ == it.cur_; }

    bool operator!=(const iterator& it) const { return cur_ != it.cur_; }
};

template<class Value, class Key, class HashFcn,
        class ExtractKey, class EqualKey, class Alloc>
struct HashTableConstIterator {
    typedef HashTable<Value, Key, HashFcn,
            ExtractKey, EqualKey, Alloc> HashTable;
    typedef HashTableConstIterator<Value, Key, HashFcn,
            ExtractKey, EqualKey, Alloc> const_iterator;
    typedef HashTableNode<Value> Node;
    typedef HashTableNode<Value>* NodePtr;

    typedef std::forward_iterator_tag iterator_category;
    typedef Value value_type;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef const Value& reference;
    typedef const Value* pointer;

    const Node* cur_; // not equal to "const NodePtr"
    const HashTable* ht_;

    HashTableConstIterator(const Node* n, const HashTable* tab) : cur_(n), ht_(tab) {}

    HashTableConstIterator() = default;

    reference operator*() const { return cur_->val_; }

    pointer operator->() const { return &(cur_->val_); }

    const_iterator& operator++() {
        const Node* old = cur_;
        cur_ = cur_->next_;
        if (!cur_) {
            size_type bucket = ht_->bkt_num(old->val_);
            while (!cur_ && ++bucket < ht_->buckets_.size()) {
                cur_ = ht_->buckets_[bucket];
            }
        }
        return *this;
    }

    const_iterator operator++(int) {
        const_iterator tmp = *this;
        ++*this;
        return tmp;
    }

    bool operator==(const const_iterator& it) const { return cur_ == it.cur_; }

    bool operator!=(const const_iterator& it) const { return cur_ != it.cur_; }
};

// the table size is primes.
static const int num_primes = 29;
static const unsigned long prime_list[num_primes] =
        {
                5ul, 53ul, 97ul, 193ul, 389ul,
                769ul, 1543ul, 3079ul, 6151ul, 12289ul,
                24593ul, 49157ul, 98317ul, 196613ul, 393241ul,
                786433ul, 1572869ul, 3145739ul, 6291469ul, 12582917ul,
                25165843ul, 50331653ul, 100663319ul, 201326611ul, 402653189ul,
                805306457ul, 1610612741ul, 3221225473ul, 4294967291ul
        };

// when we enlarge the size, we can use this function get the children prime.
inline unsigned long next_prime(unsigned long n) {
    const unsigned long* first = prime_list;
    const unsigned long* last = prime_list + num_primes;
    const unsigned long* pos = lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;
}

template<class Value, class Key, class HashFcn,
        class ExtractKey, class EqualKey, class Alloc>
class HashTable {
public:
    typedef HashFcn hasher;
    typedef Value value_type;
    typedef Key key_type;
    typedef EqualKey key_equal;

    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;

private:
    hasher hash_;
    EqualKey equals_;
    ExtractKey getKey_;

    typedef HashTableNode<Value> Node;
    typedef HashTableNode<Value>* NodePtr;

    typedef typename Alloc::template rebind<Value>::other ValueAlloc;
    typedef std::allocator_traits<ValueAlloc> ValueAllocTraits;
    typedef typename ValueAllocTraits::template rebind_alloc<Node> NodeAlloc;
    typedef typename ValueAllocTraits::template rebind_alloc<NodePtr> NodePtrAlloc;
    typedef std::allocator_traits<NodeAlloc> NodePtrAllocTraits;

    // notice that vector allocator is std::allocator<Node*>
    vector<NodePtr, NodePtrAlloc> buckets_;
    size_type num_elements_;

public:

    typedef HashTableIterator<Value, Key, HashFcn,
            ExtractKey, EqualKey, Alloc> iterator;

    typedef HashTableConstIterator<Value, Key, HashFcn,
            ExtractKey, EqualKey, Alloc> const_iterator;

    // iterator will access the private variable bucket
    friend struct HashTableIterator<Value, Key, HashFcn,
            ExtractKey, EqualKey, Alloc>;
    friend struct HashTableConstIterator<Value, Key, HashFcn,
            ExtractKey, EqualKey, Alloc>;

public:
    size_type bucketCount() const { return buckets_.size(); }

    size_type maxBucketCount() const {
        return prime_list[num_primes - 1];
    }

    hasher hashFunc() const { return hash_; }

    key_equal keyEq() const { return equals_; }

protected:
    NodeAlloc nalloc;

    NodePtr newNode(const Value& obj) {
        NodePtr n = NodePtrAllocTraits::allocate(nalloc, 1);
        n->next_ = nullptr;
        NodePtrAllocTraits::construct(nalloc, &n->val_, obj);
        return n;
    }

    void deleteNode(NodePtr n) {
        NodePtrAllocTraits::destroy(nalloc, n);
        NodePtrAllocTraits::deallocate(nalloc, n, 1);
    }

public:
    HashTable(size_type n, const HashFcn& hf, const EqualKey& eql)
            : hash_(hf), equals_(eql), getKey_(ExtractKey()), num_elements_(0) {
        initialize_buckets(n);
    }

    void initialize_buckets(size_type n) {
        // get the prime that last to n
        const size_type n_buckets = next_size(n);
        buckets_.reserve(n_buckets); // init size
        buckets_.insert(buckets_.end(), n_buckets, (NodePtr) nullptr);
        num_elements_ = 0;
    }

    size_type next_size(size_type n) const { return next_prime(n); }

    size_type size() const { return num_elements_; }

    size_type max_size() const { return size_type(-1); }

    bool empty() const { return size() == 0; }

    // Notice that there is no end of hashtable actually.
    // Here return a nullptr.
    // You cannot use the end() as the traversal upper bound.
    iterator end() { return iterator(nullptr, nullptr); }

    const_iterator end() const { return const_iterator(nullptr, nullptr); }

    iterator begin() {  // no const
        // the first element with node from left to right
        for (size_type i = 0; i < buckets_.size(); i++) {
            if (buckets_[i]) {
                return iterator(buckets_[i], this);
            }
        }
        return end();
    }

    const_iterator begin() const {
        for (size_type i = 0; i < buckets_.size(); i++) {
            if (buckets_[i]) {
                return const_iterator(buckets_[i], this);
            }
        }
        return end();
    }

    // find the element of specific key
    iterator find(const Key& key) {
        size_type n = bkt_num_key(key);
        NodePtr first = buckets_[n];
        while (first != nullptr && !equals_(getKey_(first->val_), key)) {
            first = first->next_;
        }
        return iterator(first, this);
    }

    const_iterator find(const Key& key) const {
        size_type n = bkt_num_key(key);
        NodePtr first = buckets_[n];
        while (first != nullptr && !equals_(getKey_(first->val_), key)) {
            first = first->next_;
        }
        return const_iterator(first, this);
    }

    // erase elements.
    // Two different operation:
    //  1. consider the second node to the last node of bucket
    //  2. consider the first node
    // Return the number of erased node
    size_type erase(const Key& key) {
        size_type n = bkt_num_key(key);
        NodePtr first = buckets_[n];
        size_type erased = 0;
        if (first) {
            NodePtr cur = first;
            NodePtr next = cur->next_;
            while (next) {
                if (equals_(getKey_(next->val_), key)) {
                    cur->next_ = next->next_;
                    deleteNode(next);
                    next = cur->next_;
                    erased++;
                    num_elements_--;
                } else {
                    cur = next;
                    next = cur->next_;
                }
            }
            if (equals_(getKey_(first->val_), key)) {
                buckets_[n] = first->next_;
                deleteNode(first);
                erased++;
                num_elements_--;
            }
        }
        return erased;
    }

    void erase(const iterator& it) {
        NodePtr p = it.cur_;
        if (p) {
            const size_type n = bkt_num(p->val_);
            NodePtr cur = buckets_[n];
            if (cur == p) {
                buckets_[n] = cur->next_;
                deleteNode(cur);
                --num_elements_;
            } else {
                NodePtr next = cur->next_;
                while (next) {
                    if (next == p) {
                        cur->next_ = next->next_;
                        deleteNode(next);
                        --num_elements_;
                        break;
                    } else {
                        cur = next;
                        next = cur->next_;
                    }
                }
            }
        }
    }

    void erase(iterator first, iterator last) {
        size_type fbucket = first.cur_ ? bkt_num(first.cur_->val_) : buckets_.size();
        size_type lbucket = last.cur_ ? bkt_num(last.cur_->val_) : buckets_.size();
        if (first.cur_ == last.cur_) {
            return;
        } else if (fbucket == lbucket) { // first and last in the same bucket
            _eraseBucket(fbucket, first.cur_, last.cur_);
        } else { // first and last in difference buckets
            _eraseBucket(fbucket, first.cur_, nullptr); // delete the first bucket
            for (size_type n = fbucket + 1; n < lbucket; ++n) { // delete the middle bucket
                _eraseBucket(n, nullptr);
            }
            if (lbucket != buckets_.size()) { // delete the element until last.cur in last bucket
                _eraseBucket(lbucket, last.cur_);
            }
        }
    }

    void erase(const_iterator first, const_iterator last) {
        erase(iterator(const_cast<NodePtr>(first.cur_),
                       const_cast<HashTable*>(first.ht_)),
              iterator(const_cast<NodePtr>(last.cur_),
                       const_cast<HashTable*>(last.ht_))
        );
    }

    size_type count(const Key& key) const {
        const size_type n = bkt_num_key(key);
        NodePtr first = buckets_[n];
        size_type res = 0;

        while (first) {
            if (equals_(getKey_(first->val_), key)) {
                res++;
            }
            first = first->next_;
        }
        return res;
    }

public:
    // insert unique with resize
    pair<iterator, bool> insertUnique(const Value& obj) {
        resize(num_elements_ + 1);
        return insertUniqueNoresize(obj);
    }

    template <class InputIterator>
    void insertUnique(InputIterator first, InputIterator last) {
        for(; first != last; ++first) {
            insertUnique(*first);
        }
    }

    // check the size and resize
    void resize(size_type hint) {
        const size_type old_size = buckets_.size();
        if (hint > old_size) {
            const size_type new_size = next_size(hint);
            if (new_size > old_size) {
                vector<NodePtr, NodePtrAlloc> tmp(new_size, (NodePtr) nullptr); // new vector
                for (size_type bucket = 0; bucket < old_size; ++bucket) {
                    NodePtr first = buckets_[bucket];
                    while (first) {
                        // get the new position
                        size_type new_bucket = bkt_num(first->val_, new_size);
                        // move the children node to first position of current position
                        buckets_[bucket] = first->next_;
                        // the children pointer links to the first node of new bucket.
                        first->next_ = tmp[new_bucket];
                        // change the fisrt node of new bucket to current node.
                        tmp[new_bucket] = first;
                        // turn to children node
                        first = buckets_[bucket];
                    }
                }
                // swap old and new vector.
                // local variable tmp (old) will be freed.
                buckets_.swap(tmp);
            }
        }
    }

    pair<iterator, bool> insertUniqueNoresize(const Value& obj) {
        const size_type n = bkt_num(obj);
        NodePtr first = buckets_[n];
        // check whether the same node existed or not.
        for (NodePtr cur = first; cur; cur = cur->next_) {
            if (equals_(getKey_(cur->val_), getKey_(obj))) {
                return pair<iterator, bool>(iterator(cur, this), false);
            }
        }
        // first point to first position
        NodePtr tmp = newNode(obj);
        tmp->next_ = first;
        buckets_[n] = tmp; // become the new first node.
        ++num_elements_;
        return pair<iterator, bool>(iterator(tmp, this), true);
    }

    // allow replaced key
    iterator insertEqual(const Value& obj) {
        resize(num_elements_ + 1);
        return insert_equal_noresize(obj);
    }

    template <class InputIterator>
    void insertEqual(InputIterator first, InputIterator last) {
        for(; first != last; ++first) {
            insertEqual(*first);
        }
    }

    iterator insert_equal_noresize(const Value& obj) {
        const size_type n = bkt_num(obj);
        NodePtr first = buckets_[n];
        for (NodePtr cur = first; cur; cur = cur->next_) {
            if (equals_(getKey_(cur->val_), getKey_(obj))) {
                NodePtr tmp = newNode(obj);
                tmp->next_ = cur->next_;  // insert to the back of same element
                cur->next_ = tmp;
                ++num_elements_;
                return iterator(tmp, this);
            }
        }
        // no same key
        NodePtr tmp = newNode(obj);
        tmp->next_ = first;
        buckets_[n] = tmp;
        ++num_elements_;
        return iterator(tmp, this);
    }

    size_type bkt_num(const Value& obj, size_type n) const {
        return bkt_num_key(getKey_(obj), n);
    }

    size_type bkt_num(const Value& obj) const {
        return bkt_num_key(getKey_(obj));
    }

    size_type bkt_num_key(const Key& key) const {
        return bkt_num_key(key, buckets_.size());
    }

    size_type bkt_num_key(const Key& key, size_type n) const {
        return hash_(key) % n;
    }

public:
    void clear() {
        for (size_type i = 0; i < buckets_.size(); i++) {
            NodePtr cur = buckets_[i];
            while (cur != 0) {
                NodePtr next = cur->next_;
                deleteNode(cur);
                cur = next;
            }
            buckets_[i] = 0;
        }
        num_elements_ = 0;

        // you do not need to free the vector
    }

    // a deep copy function that copy data from other hashtable
    void copyFrom(const HashTable& ht) {
        buckets_.clear();
        buckets_.reserve(ht.buckets_.size());
        // After clear, end() is equal to begin()
        buckets_.insert(buckets_.end(), ht.buckets_.size(), (NodePtr) nullptr);
        for (size_type i = 0; i < ht.buckets_.size(); i++) {
            if (const NodePtr cur = ht.buckets_[i]) {
                // copy the first node
                NodePtr copy = newNode(cur->val_);
                buckets_[i] = copy;
                // copy the following node.
                for (NodePtr next = cur->next_; next; cur = next, next = cur->next_) {
                    copy->next_ = newNode(next->val_);
                    copy = copy->next_;
                }
            }
        }
        num_elements_ = ht.num_elements_;
    }

    size_type elemsInBucket(size_type bucket) const {
        size_type res = 0;
        for (NodePtr n = buckets_[bucket]; n; n = n->next_) {
            res += 1;
        }
        return res;
    }

private:
    void _eraseBucket(const size_type n, NodePtr last) {
        NodePtr cur = buckets_[n];
        while (cur != last) {
            NodePtr next = cur->next_;
            deleteNode(cur);
            cur = next;
            buckets_[n] = cur;
            --num_elements_;
        }
    }

    void _eraseBucket(const size_type n, NodePtr first, NodePtr last) {
        NodePtr cur = buckets_[n];
        if (cur == first) {
            _eraseBucket(n, last);
        } else {
            NodePtr next;
            for (next = cur->next_; next != first; cur = next, next = cur->next_);
            while (next != last) {
                cur->next_ = next->next_;
                deleteNode(next);
                next = cur->next_;
                --num_elements_;
            }
        }
    }
};

}

#endif //ALG_HASHTABLE_H
