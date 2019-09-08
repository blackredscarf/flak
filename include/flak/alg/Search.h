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


// lower bound:
//  find the first position that greater/smaller than or equal to value.
//  For example, as for ascending, 21 insert to {11, 15, 20, 21, 21, 23, 24},
//  will become {11, 15, 20, (21), 21, 21, 23, 24}
//
// upper bound:
//  find the last position that greater/smaller than value.
//  For example, as for ascending, 21 insert to {11, 15, 20, 21, 21, 23, 24},
//  will become {11, 15, 20, 21, 21, (21), 23, 24}

#ifndef ALG_BOUND_H
#define ALG_BOUND_H

#include <iterator>
using std::__iterator_category;
using std::forward_iterator_tag;
using std::random_access_iterator_tag;
using std::iterator_traits;


namespace flak {
template<typename ForwardIterator, typename T, typename Compare>
ForwardIterator _lowerBound(ForwardIterator first,
                            ForwardIterator last,
                            const T &value,
                            forward_iterator_tag,
                            Compare comp) {
    typedef typename iterator_traits<ForwardIterator>::difference_type size_type;
    size_type len = std::distance(first, last);
    size_type half;
    ForwardIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        std::advance(middle, half); // middle + half
        if (comp(*middle, value)) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}

template<typename RandomAccessIterator, typename T, typename Compare>
RandomAccessIterator _lowerBound(RandomAccessIterator first,
                                 RandomAccessIterator last,
                                 const T &value, random_access_iterator_tag, Compare comp) {
    typedef typename iterator_traits<RandomAccessIterator>::difference_type size_type;
    size_type len = last - first;
    size_type half;
    RandomAccessIterator middle;

    while (len > 0) {
        half = len >> 1;
        middle = first + half;
        if (comp(*middle, value)) {
            first = middle + 1;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}

template<typename ForwardIterator, typename T, typename Compare>
ForwardIterator lowerBound(ForwardIterator first,
                           ForwardIterator last,
                           const T &value, Compare comp) {
    return _lowerBound(first, last, value, __iterator_category(first), comp);
}


template<typename ForwardIterator, typename T>
ForwardIterator lowerBound(ForwardIterator first,
                           ForwardIterator last,
                           const T &value) {
    return lowerBound(first, last, value, std::less<T>());
}

template<typename ForwardIterator, typename T, typename Compare>
ForwardIterator _upperBound(ForwardIterator first,
                            ForwardIterator last,
                            const T &value, forward_iterator_tag,
                            Compare comp) {
    typedef typename iterator_traits<ForwardIterator>::difference_type size_type;
    size_type len = std::distance(first, last);
    size_type half;
    ForwardIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        std::advance(middle, half); // middle + half
        if (!comp(value, *middle)) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}

template<typename RandomAccessIterator, typename T, typename Compare>
RandomAccessIterator _upperBound(RandomAccessIterator first,
                                 RandomAccessIterator last,
                                 const T &value, random_access_iterator_tag, Compare comp) {
    typedef typename iterator_traits<RandomAccessIterator>::difference_type size_type;
    size_type len = last - first;
    size_type half;
    RandomAccessIterator middle;

    while (len > 0) {
        half = len >> 1;
        middle = first + half;
        if (!comp(value, *middle)) {
            first = middle + 1;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}

template<typename ForwardIterator, typename T, typename Compare>
ForwardIterator upperBound(ForwardIterator first,
                           ForwardIterator last,
                           const T &value, Compare comp) {
    return _upperBound(first, last, value, __iterator_category(first), comp);
}


template<typename ForwardIterator, typename T>
ForwardIterator upperBound(ForwardIterator first,
                           ForwardIterator last,
                           const T &value) {
    return upperBound(first, last, value, std::less<T>());
}

template<typename ForwardIterator, typename T, typename Compare>
bool binarySearch(ForwardIterator first,
                  ForwardIterator last,
                  const T &value, Compare comp) {
    ForwardIterator i = lowerBound(first, last, value, comp);
    return i != last && !comp(value, *i);
}


template<typename ForwardIterator, typename T>
bool binarySearch(ForwardIterator first,
                  ForwardIterator last,
                  const T &value) {
    return binarySearch(first, last, value, std::less<T>());
}

}
#endif //ALG_BOUND_H
