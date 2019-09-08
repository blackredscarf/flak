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

// Trie.
// You can learn it at https://en.wikipedia.org/wiki/Trie.
/*
 *  insert [apple, bear, apply, apace, app]
 *
 *                         header
 *                      /    /   \
 *                     a    b(1)  c ...
 *                    /     \
 *                   p(4)    e(1)
 *                  / \       \
 *                 /   \       \
 *                /     \       \
 *            p(3,end)   a       a(1)
 *               /       \       \
 *              l(2)   e(1,end)  r(1,end)
 *             / \
 *            /   \
 *        e(1,end) y(1,end)
 *
 */

#ifndef FLAK_TRIE_H
#define FLAK_TRIE_H
#include <map>
#include <utility>
#include <iterator>
#include <vector>
#include <exception>
#include <stack>
#include <cassert>
#include "TypeUtils.h"
#include <map>

using std::make_tuple;
using std::pair;
using std::tuple;
using std::make_pair;
using std::map;
using std::stack;
using std::forward_iterator_tag;
using std::iterator_traits;


namespace flak {

template <typename Key, typename Val, typename Size = size_t>
class _QueryPrefix;

template<typename Key, typename Val, typename Size = size_t>
struct TrieNode {
    typedef TrieNode<Key, Val, Size>* SelfPtr;
    Size num_;
    Val value_;
    bool end_;
    map<Key, SelfPtr> children;

    TrieNode() {}
    ~TrieNode() {}

    explicit TrieNode(Size num) : num_(num), end_(false) {}

    SelfPtr& operator[](const Key& key) {
        return children[key];
    }

    Size count(const Key& key) const {
        return children.count(key);
    }
};

template<typename Key, typename Val, typename Size = size_t>
class TrieIterator {
public:
    typedef Size size_type;
    typedef Key key_type;
    typedef Val value_type;
    typedef Val* pointer;
    typedef Val& reference;
    typedef ptrdiff_t difference_type;

private:
    typedef TrieNode<Key, Val, Size> Node;
    typedef TrieNode<Key, Val, Size>* NodePtr;
public:
    NodePtr node_;

    explicit TrieIterator(const NodePtr node) : node_(node) {}

    reference operator*() const { return node_->value_; }

    pointer operator->() const { return &(operator*()); }

    size_type num() const { return node_->num_; }

};

template<typename Key, typename Val, typename Size = size_t>
class Trie {
    typedef Size size_type;

    // Use traits to get the element type of key
    typedef typename ContainerTraits<Key>::value_type key_type;
    typedef TrieNode<key_type, Val, Size> Node;
    typedef TrieNode<key_type, Val, Size>* NodePtr;
    typedef _QueryPrefix<Key, Val, Size> Query;
    typedef Trie<Key, Val, Size> Self;

private:
    NodePtr header_;
    NodePtr& child(NodePtr x, const key_type& key) const { return x->children[key]; }

public:
    friend Query;
    typedef TrieIterator<key_type, Val, Size> iterator;

    Trie() : header_(new Node()) {}
    ~Trie() { delete header_; }
    void clear() {
        delete header_;
        header_ = new Node();
    }

    // you cannot use this function for the pointer.
    // because begin() cannot find the start of the pointer.
    // you can use insert(InputIterator first, InputIterator last, const Val& value).
    void insert(const Key& key, const Val& value) {
        insert(std::begin(key), std::end(key), value);
    }

    // The function does not check if exist. New value overlaps the old value.
    template<typename InputIterator>
    void insert(InputIterator first, InputIterator last, const Val& value) {
        if (first == last) {
            return;
        }
        NodePtr cur = header_;
        for (; first != last; ++first) {
            if(cur == nullptr) {
                cur = new Node(0);
            }
            if (cur->count(*first) == 0) {
                child(cur, *first) = new Node(1);
            }else if (cur->count(*first) > 0) {
                child(cur, *first)->num_++;
            }
            cur = child(cur, *first);
        }
        cur->end_ = true;
        cur->value_ = value;
    }

    // you cannot use this function for the pointer.
    // you can use find(InputIterator first, InputIterator last).
    pair<bool, iterator> find(const Key& key) const {
        return find(std::begin(key), std::end(key));
    }

    // This function does not find the prefix.
    // Using the key to find the value as same as the find of map.
    template<typename InputIterator>
    pair<bool, iterator> find(InputIterator first, InputIterator last) const {
        NodePtr cur = header_;
        for (; first != last; ++first) {
            if (cur->count(*first) > 0) {
                cur = child(cur, *first);
            } else {
                return make_pair(false, iterator(nullptr));
            }
        }
        if(!cur->end_) {
            return make_pair(false, iterator(nullptr));
        }
        return make_pair(true, iterator(cur));
    }

    template<typename InputIterator>
    std::vector<iterator> findPrefix(InputIterator first, InputIterator last) const {
        Query q(this, first, last);
        return q.findAll();
    }

    std::vector<iterator> findPrefix(const Key& prefix) const {
        return findPrefix(std::begin(prefix), std::end(prefix));
    }

    pair<bool, size_type> existPrefix(const Key& prefix) const {
        return existPrefix(std::begin(prefix), std::end(prefix));
    }

    template<typename InputIterator>
    pair<bool, size_type> existPrefix(InputIterator first, InputIterator last) const {
        NodePtr cur = header_;
        for (; first != last; ++first) {
            if (cur->count(*first) > 0) {
                cur = child(cur, *first);
            } else {
                return make_pair(false, 0);
            }
        }
        return make_pair(true, cur->num_);
    }

    // you cannot use this function for the pointer.
    // you can use erase(InputIterator first, InputIterator last).
    bool erase(const Key& key) {
        return erase(std::begin(key), std::end(key));
    }

    template<typename InputIterator>
    bool erase(InputIterator first, InputIterator last) {
        NodePtr cur = header_;
        NodePtr parent = nullptr;
        stack<tuple<NodePtr, NodePtr, key_type>> s;
        for (; first != last; ++first) {
            if (cur->count(*first) > 0) {
                parent = cur;
                cur = child(cur, *first);
                s.push(make_tuple(cur, parent, *first));
            } else {
                return false;
            }
        }
        if(!cur->end_) {    // you cannot delete a prefix
            return false;
        }
        while(!s.empty()) {
            tuple<NodePtr, NodePtr, key_type> v = s.top();
            s.pop();
            NodePtr now = std::get<0>(v);
            NodePtr p = std::get<1>(v);
            key_type k = std::get<2>(v);
            --now->num_;
            if(now->num_ < 1) {
                p->children.erase(k);
                delete now;
            }
        }
        return true;
    }

    // you cannot use this function for the pointer.
    // you can use erasePrefix(InputIterator first, InputIterator last).
    bool erasePrefix(const Key& key) {
        return erasePrefix(std::begin(key), std::end(key));
    }

    template<typename InputIterator>
    bool erasePrefix(InputIterator first, InputIterator last) {

        stack<tuple<NodePtr, NodePtr, key_type>> s;
        NodePtr lastNode = header_;
        NodePtr parent = nullptr;
        key_type k;

        // find the last node of the prefix
        for(; first != last; ++first) {
            if(lastNode != nullptr && lastNode->count(*first) > 0) {
                parent = lastNode;
                k = *first;
                lastNode = child(lastNode, *first);
                s.push(make_tuple(lastNode, parent, *first));
            } else {
                return false;
            }
        }

        // remove all node of the prefix sub-tree in recursion, and return the number of leaves
        size_type erasedNum = _erasePrefixLoop(lastNode);

        // come back and percolate up, substract the number of leaves in every node
        while(!s.empty()) {
            tuple<NodePtr, NodePtr, key_type> now = s.top();
            s.pop();
            NodePtr cur = std::get<0>(now);
            parent = std::get<1>(now);
            key_type key = std::get<2>(now);
            cur->num_ -= erasedNum;
            assert((cur->num_ >= 0));
            if(cur->num_ == 0) {
                parent->children.erase(key);
                delete cur;
            }
        }
        return true;
    }

private:
    size_type _erasePrefixLoop(NodePtr cur) {
        size_type sum = 0;
        if(cur->end_) {
            sum += 1;
        }
        auto start = cur->children.begin();
        auto end = cur->children.end();
        while(start != end) {
            auto& p = *start;
            sum += _erasePrefixLoop(cur->children[p.first]);
            delete cur->children[p.first];
            start = cur->children.erase(start);
        }
        return sum;
    }
};

template <typename Key, typename Val, typename Size>
class _QueryPrefix {
    typedef Size size_type;
    typedef typename ContainerTraits<Key>::value_type key_type;
    typedef TrieNode<key_type, Val, Size> Node;
    typedef TrieNode<key_type, Val, Size>* NodePtr;
    typedef Trie<Key, Val, Size> Trie;

public:
    typedef TrieIterator<key_type, Val, Size> iterator;
private:
    pair<bool, iterator> nowRoot;
    std::vector<iterator> all;
    const Trie* trie_;
public:
    template<typename InputIterator>
    explicit _QueryPrefix(const Trie* trie, InputIterator prefixFirst, InputIterator prefixLast)
        : trie_(trie), nowRoot(std::make_pair(false, iterator(nullptr))) {

        NodePtr cur = trie_->header_;
        for (; prefixFirst != prefixLast; ++prefixFirst) {
            if (cur->count(*prefixFirst) > 0) {
                cur = cur->children[*prefixFirst];
            } else {
                nowRoot = make_pair(false, iterator(nullptr));
            }
        }
        nowRoot = make_pair(true, iterator(cur));
    }

    std::vector<iterator> findAll() {
        if(!nowRoot.first) {
            return all;
        }
        _findAllLoop(nowRoot.second.node_);
        return all;
    }

private:
    void _findAllLoop(NodePtr cur) {
        if(cur->end_) {
            all.push_back(iterator(cur));
        }
        if(cur->children.empty()) {
            return;
        }
        for(const auto& p : cur->children) {
            _findAllLoop(cur->children[p.first]);
        }
    }
};

}

#endif //FLAK_TRIE_H
