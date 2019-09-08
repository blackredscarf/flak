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

#ifndef ALG_PRIORITYQUEUE_H
#define ALG_PRIORITYQUEUE_H

#include <vector>
#include <functional>
#include "Heap.h"
using std::vector;
using std::less;

namespace flak {

template<class T, class Sequence = vector<T>,
        class Compare = less<typename Sequence::value_type>>
class PriorityQueue {
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c_;
    Compare comp_;

public:
    PriorityQueue() {}

    explicit PriorityQueue(const Compare &x) : c_(), comp_(x) {}

    template<class InputIterator>
    PriorityQueue(InputIterator first, InputIterator last, const Compare &x)
            : c_(first, last), comp_(x) {
        makeHeap(c_.begin(), c_.end(), comp_);
    }

    template<class InputIterator>
    PriorityQueue(InputIterator first, InputIterator last)
            : c_(first, last) {
        makeHeap(c_.begin(), c_.end(), comp_);
    }

    bool empty() const { return c_.empty(); }

    size_type size() const { return c_.size(); }

    const_reference top() const { return c_.front(); }

    void push(const value_type &x) {
        c_.push_back(x);
        pushHeap(c_.begin(), c_.end(), comp_);
    }

    void pop() {
        popHeap(c_.begin(), c_.end(), comp_);
        c_.pop_back();
    }
};

}
#endif //ALG_PRIORITYQUEUE_H
