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

#ifndef ALG_INSERTIONSORT_H
#define ALG_INSERTIONSORT_H

#include <iterator>

using std::iterator_traits;

namespace flak {

template<class RandomAccessIterator, class T, class Compare>
void _unguardedLinearInsert(RandomAccessIterator last, T value, Compare comp) {
    RandomAccessIterator next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

template<class RandomAccessIterator, class Compare>
void _linearInsert(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type T;
    T value = *last;
    if (comp(value, *first)) { // *first always is the smallest value
        std::copy_backward(first, last, last + 1);
        *first = value;
    } else {
        _unguardedLinearInsert(last, value, comp);
    }
}


template<class RandomAccessIterator, class Compare>
void insertionSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if (first == last) {
        return;
    }
    for (RandomAccessIterator i = first + 1; i != last; ++i) {
        _linearInsert(first, i, comp); // [first, i)
    }
}

template<class RandomAccessIterator>
void insertionSort(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type T;
    insertionSort(first, last, std::less<T>());
}

template<class RandomAccessIterator, class Compare>
void _unguardedInsertionSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type T;
    for (RandomAccessIterator i = first; i != last; ++i) {
        _unguardedLinearInsert(i, T(*i), comp);
    }
}

}
#endif //ALG_INSERTIONSORT_H
