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

#ifndef ALG_SORT_H
#define ALG_SORT_H

#include "QuickSort.h"
#include "InsertionSort.h"
#include "../Heap.h"

namespace flak {

// find the maximum k that 2^k <= n
template<class Size>
inline Size _lg(Size n) {
    Size k;
    for (k = 0; n > 1; n >>= 1) ++k;
    return k;
}

// Only sort the range of [first, middle).
// When compare function is less<>, we make a max-heap in [first, middle).
// The first element of heap always is the max value of [first, middle),
// We use it compare to elements of [middle, last) in iteration.
// If *first is greater than one target, we pop the *first and push target in heap.
// Finally, we just sort the heap with less<>, then we get the ascending order sequence [first, middle).
template<class RandomAccessIterator, class Compare>
void _partialSort(RandomAccessIterator first, RandomAccessIterator middle,
                  RandomAccessIterator last, Compare comp) {
    makeHeap(first, middle, comp);
    for (RandomAccessIterator i = middle; i < last; ++i) {
        if (comp(*i, *first)) {
            // *frist always is the greatest/smallest value
            // our purpose is to move the great/small value out of [first, middle)
            popHeap(first, middle, i, *i, comp);
        }
    }
    sortHeap(first, middle, comp);
}

// Only sort the range of [first, middle).
// O((n-k)*log(k))
template<class RandomAccessIterator, class Compare>
void partialSort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare comp) {
    _partialSort(first, middle, last, comp);
}

template<class RandomAccessIterator>
void partialSort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
    partialSort(first, middle, last, std::less<>());
}

const int _threshold = 16;

// [first] is the first element of a range.
// [last] is the last element of a range.
// [depthLimit] is the maximum number of allowed recursions, if exceed, using heap sort.
template<class RandomAccessIterator, class Size, class Compare>
void _introsortLoop(RandomAccessIterator first,
                    RandomAccessIterator last,
                    Size depthLimit, Compare comp) {

    typedef typename iterator_traits<RandomAccessIterator>::value_type T;

    // When the distance of last and first is larger than threshold,
    // we use partialSort + quickSort
    while (last - first > _threshold) {
        // use the heap sort to sort this range when exceed depthLimit
        if (depthLimit == 0) {
            partialSort(first, last, last, comp);
            return;
        }

        // had cutted a time
        --depthLimit;

        //  quick sort
        RandomAccessIterator cut = _unguard_partition(first, last,
                                                      T(_median(*first, *(first + (last - first) / 2), *(last - 1))));

        // recursion
        _introsortLoop(cut, last, depthLimit, comp);

        // left half range
        last = cut;
    }
}

// do the insertion sort
template<class RandomAccessIterator, class Compare>
void _finalInsertionSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if (last - first > _threshold) {
        insertionSort(first, first + _threshold, comp);
        _unguardedInsertionSort(first + _threshold, last, comp);
    } else {
        insertionSort(first, last, comp);
    }
}

template<class RandomAccessIterator, class Compare>
void Sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if (first != last) {
        _introsortLoop(first, last, _lg(last - first) * 2, comp);
        // after intro sort, the seq is in almost order,
        // then insertion sort will have a excellent performance
        _finalInsertionSort(first, last, comp);
    }
}

template<class RandomAccessIterator>
void Sort(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type T;
    Sort(first, last, std::less<T>());
}

}
#endif //ALG_SORT_H
