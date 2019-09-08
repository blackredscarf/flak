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

// Conceptually, a merge sort works as follows:
//
//  1. Divide the unsorted list into n sublists, each containing one element (a list of one element is considered sorted).
//  2. Repeatedly merge sublists to produce new sorted sublists until there is only one sublist remaining. This will be the sorted list.

#ifndef ALG_MERGESORT_H
#define ALG_MERGESORT_H

#include "Merge.h"
#include <iterator>
using std::iterator_traits;

namespace flak {

template<class BidirectionalIter, class Compare>
void mergeSort(BidirectionalIter first, BidirectionalIter last, Compare comp) {
    typedef typename iterator_traits<BidirectionalIter>::difference_type size_type;
    size_type n = std::distance(first, last);
    if (n == 0 || n == 1) {
        return;
    } else {
        BidirectionalIter mid = first + n / 2;
        mergeSort(first, mid, comp);
        mergeSort(mid, last, comp);

        // best space complexity
        inplaceMerge(first, mid, last, comp);

//        // higher space complexity
//        typedef typename iterator_traits<BidirectionalIter>::value_type T;
//        std::_Temporary_buffer<BidirectionalIter, T> bufLeft(first, mid);
//        T* bufferLeft = bufLeft.begin();
//        T* endBufferLeft = std::copy(first, mid, bufferLeft);
//        Merge(bufferLeft, endBufferLeft, mid, last, first, comp);

//        // highest space complexity
//        typedef typename iterator_traits<BidirectionalIter>::value_type T;
//        std::_Temporary_buffer<BidirectionalIter, T> bufLeft(first, mid);
//        std::_Temporary_buffer<BidirectionalIter, T> bufRight(mid, last);
//        T* bufferLeft = bufLeft.begin();
//        T* bufferRight = bufRight.begin();
//        T* endBufferLeft = std::copy(first, mid, bufferLeft);
//        T* endBufferRight = std::copy(mid, last, bufferRight);
//        Merge(bufferLeft, endBufferLeft, bufferRight, endBufferRight, first, comp);
    }
}

template<class BidirectionalIter>
void mergeSort(BidirectionalIter first, BidirectionalIter last) {
    typedef typename iterator_traits<BidirectionalIter>::difference_type size_type;
    typedef typename iterator_traits<BidirectionalIter>::value_type T;
    mergeSort(first, last, std::less<T>());
}

}
#endif //ALG_MERGESORT_H
