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

// You can learn it at https://en.wikipedia.org/wiki/Quickselect

#ifndef FLAK_RANDOMIZEDSELECT_H
#define FLAK_RANDOMIZEDSELECT_H

#include "Alg.h"
#include <assert.h>
#include <utility>
using std::make_pair;

namespace flak {

template <typename Value, typename Compare>
struct _RandomPartitionFunc {
    Value pivots_;
    _RandomPartitionFunc(Value pivots) : pivots_(pivots) {}
    bool operator()(Value v) {
        return Compare()(v, pivots_);
    }
};

template <class RandomIterator, class Compare>
RandomIterator randomPartition(RandomIterator first,
                                      RandomIterator last, Compare comp) {
    typedef typename iterator_traits<RandomIterator>::value_type Val;
    int idx = rand() % (last - first);
    Val pivots = *(first + idx);
    std::iter_swap(first + idx, first);
    auto it = flak::partition(first + 1, last, _RandomPartitionFunc<Val, Compare>(pivots));
    std::iter_swap(first, --it);
    return it;
}


template <class RandomIterator>
RandomIterator randomPartition(RandomIterator first,
                                      RandomIterator last) {
    typedef typename iterator_traits<RandomIterator>::value_type Val;
    return randomPartition(first, last, std::less<Val>());
}

template <class RandomIterator, class Compare>
RandomIterator randomizedSelect(RandomIterator first,
                                      RandomIterator last,
                                      size_t k, Compare comp) {
    if(first == last) {
        return first;
    }

    assert((k > 0));
    assert((k <= last - first));
    assert((first < last));

    auto it = randomPartition(first, last, comp);
    size_t frontNum = it - first + 1;

    if(frontNum == k) {
        return it;
    } else if(frontNum > k) {
        return randomizedSelect(first, it + 1, k, comp);
    } else {
        return randomizedSelect(it + 1, last, k - frontNum, comp);
    }

}

template <class RandomIterator>
RandomIterator randomizedSelect(RandomIterator first,
                                                 RandomIterator last,
                                       size_t k) {
    typedef typename iterator_traits<RandomIterator>::value_type Val;
    return randomizedSelect(first, last, k, std::less<Val>());
}

}



#endif //FLAK_RANDOMIZEDSELECT_H
