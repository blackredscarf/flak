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


// Merge: merge two order list to one order list

#ifndef ALG_MERGE_H
#define ALG_MERGE_H

#include <iterator>
#include <memory>
#include <exception>
using std::iterator_traits;

namespace flak {
template<class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter Merge(InputIter1 first1, InputIter1 last1,
                 InputIter2 first2, InputIter2 last2, OutputIter result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return std::copy(first2, last2, std::copy(first1, last1, result));
}

template<class InputIter1, class InputIter2, class OutputIter>
OutputIter Merge(InputIter1 first1, InputIter1 last1,
                 InputIter2 first2, InputIter2 last2, OutputIter result) {
    typedef typename iterator_traits<InputIter1>::value_type T;
    return Merge(first1, last1, first2, last2, result, std::less<T>());
}

template<class BidirectionalIter,
        class BidirectionalIter2, class BidirectionalIter3, class Compare>
BidirectionalIter3 MergeBackward(BidirectionalIter first1, BidirectionalIter last1,
                                 BidirectionalIter2 first2, BidirectionalIter2 last2, BidirectionalIter3 result,
                                 Compare comp) {
    if (first1 == last1) {
        return std::copy_backward(first2, last2, result);
    }
    if (first2 == last2) {
        return std::copy_backward(first1, last1, result);
    }
    --last1;
    --last2;
    while (true) {
        if (comp(*last2, *last1)) {
            *--result = *last1;
            if (first1 == last1) {
                return std::copy_backward(first2, ++last2, result);
            }
            --last1;
        } else {
            *--result = *last2;
            if (first2 == last2) {
                return std::copy_backward(first1, ++last1, result);
            }
            --last2;
        }
    }
}

template<class BidirectionalIter, class BidirectionalIter2, class BidirectionalIter3>
BidirectionalIter3 MergeBackward(BidirectionalIter first1, BidirectionalIter last1,
                                 BidirectionalIter2 first2, BidirectionalIter2 last2, BidirectionalIter3 result) {
    typedef typename iterator_traits<BidirectionalIter>::value_type T;
    return MergeBackward(first1, last1, first2, last2, result, std::less<T>());
}


template<class BidirectionalIterator, class Distance, class Pointer, class Compare>
void _mergeAdaptive(BidirectionalIterator first,
                    BidirectionalIterator middle,
                    BidirectionalIterator last,
                    Distance len1, Distance len2,
                    Pointer buffer, Distance bufferSize, Compare comp) {
    if (len1 <= len2 && len1 <= bufferSize) { // contain the range one
        Pointer endBuffer = std::copy(first, middle, buffer);
        Merge(buffer, endBuffer, middle, last, first, comp);
    } else if (len2 <= bufferSize) { // contain the range two
        Pointer endBuffer = std::copy(middle, last, buffer);
        // We cannot start with the first, because [first, middle) is unbuffered.
        MergeBackward(first, middle, buffer, endBuffer, last, comp);
    } else {
        // out of memory
        throw std::bad_alloc();
    }
}

template<class BidirectionalIterator, class Compare>
void inplaceMerge(BidirectionalIterator first,
                  BidirectionalIterator middle, BidirectionalIterator last, Compare comp) {
    if (first == middle || middle == last) return;
    typedef typename iterator_traits<BidirectionalIterator>::difference_type size_type;
    typedef typename iterator_traits<BidirectionalIterator>::value_type T;

    size_type len1 = std::distance(first, middle);
    size_type len2 = std::distance(middle, last);
    std::_Temporary_buffer<BidirectionalIterator, T> buf(first, last);
    if (buf.begin() == 0) {
        // out of memory
        throw std::bad_alloc();
    } else {
        _mergeAdaptive(first, middle, last, len1, len2, buf.begin(), buf.size(), comp);
    }
}

template<class BidirectionalIterator>
void inplaceMerge(BidirectionalIterator first,
                  BidirectionalIterator middle, BidirectionalIterator last) {
    if (first == middle || middle == last) return;
    typedef typename iterator_traits<BidirectionalIterator>::difference_type size_type;
    typedef typename iterator_traits<BidirectionalIterator>::value_type T;

    inplaceMerge(first, middle, last, std::less<T>());
}

}

#endif //ALG_MERGE_H
