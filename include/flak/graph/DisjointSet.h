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

// You can learn the disjoint-set at https://en.wikipedia.org/wiki/Disjoint-set_data_structure
//
// find: follows the chain of parent pointers from x up the tree until it reaches a root element,
//       whose parent is itself. This root element is the representative member of the set to which
//       x belongs, and may be x itself.
//
// union (merge): uses Find to determine the roots of the trees x and y belong to.
//       If the roots are distinct, the trees are combined by
//       attaching the root of one to the root of the other.
//
// set1: (1 : 2) -> (2 : 3) -> (3 : 5) -> (5 : 5)
// set2: (7: 8) -> (8 : 10) -> (10: 11) -> (11 : 11)
// find(3) = 5
// find(8) = 11
// merge(set1, set2)
//   set1: (1 : 2) -> (2 : 3) -> (3 : 5) -> (5 : 5)
//         (7 : 8) -> (8 : 10) -> (10: 11) -> (11 : 5)


#ifndef FLAK_DISJOINTSET_H
#define FLAK_DISJOINTSET_H

#include <map>
#include <utility>
#include <initializer_list>
using std::make_pair;
using std::pair;

namespace flak {

template <typename Val>
class DisjointSet {
private:
    std::map<Val, Val> parent_;
public:
    DisjointSet() = default;
    explicit DisjointSet(size_t n) : parent_(n) {};

    DisjointSet(std::initializer_list<Val> ilist) {
        for(auto& v : ilist) {
            parent_.insert(make_pair(v, v));
        }
    }

    template <typename InputIter>
    DisjointSet(InputIter first, InputIter last) {
        for(; first != last; ++first) {
            parent_.insert(make_pair(*first, *first));
        }
    }

    bool insert(const Val& v) {
        return parent_.insert(make_pair(v, v)).second;
    }

    // find the subset of [r]
    pair<bool, Val> find(const Val& r) {
        Val now = r;
        if(parent_.count(now) == 0) {
            return make_pair(false, Val());
        }
        Val cur = now;
        while (parent_[now] != now) {
            now = parent_[now];
        }
        // compress the path
        Val next;
        while(cur != now) {
            next = parent_[cur];
            parent_[cur] = now;
            cur = next;
        }
        return make_pair(true, now);
    }

    // merge two subset.
    // If v1 or v2 not in the set, return -1.
    // If merging success, return 1.
    // If merging fail, linking to a same ancestry, return 0.
    short merge(const Val& v1, const Val& v2) {
        auto p1 = find(v1);
        if(!p1.first) {
            return -1;
        }
        auto p2 = find(v2);
        if(!p2.first) {
            return -1;
        }
        const Val& v1ancestry = p1.second;
        const Val& v2ancestry = p2.second;

        if(v1ancestry != v2ancestry) {
            parent_[v2ancestry] = v1ancestry;
            return 1;
        } else {
            return 0;
        }
    }


};

}


#endif //FLAK_DISJOINTSET_H
