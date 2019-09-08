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

#ifndef ALG_QUICKSORT_H
#define ALG_QUICKSORT_H

#include <iterator>
using std::iter_swap;
using std::iterator_traits;

namespace flak {
// find the middle value
template<class T>
const T &_median(const T &a, const T &b, const T &c) {
    if (a < b) {
        if (b < c) {
            return b; // a < b < c, middle is b
        } else if (a < c) {
            return c; // a < c <= b, middle is c
        } else {
            return a;
        }
    } else if (a < c) {
        return a;
    } else {
        return b;
    }
}

template<class RandomAccessIterator, class T>
RandomAccessIterator _unguard_partition(RandomAccessIterator first,
                                        RandomAccessIterator last, T pivot) {
    while (true) {
        while (*first < pivot) ++first;
        --last;
        while (pivot < *last) --last;
        if (!(first < last)) return first;
        iter_swap(first, last);
        ++first;
    }
}

template<class RandomAccessIterator, class Compare>
void _quickSortLoop(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type T;
    if (first < last) {
//        T pivot = *(first + (last - first) / 2);
        T pivot = _median(*first, *(first + (last - first) / 2), *last);

        RandomAccessIterator left = first;
        RandomAccessIterator right = last;
        while (left < right) {
            while (comp(*left, pivot)) ++left;
            while (comp(pivot, *right)) --right;
            if (left < right) {
                iter_swap(left, right);
                ++left, --right;
            } else if (left == right) {
                ++left, --right;
            }
        }

        if (first < right) {
            _quickSortLoop(first, right, comp);
        }
        if (last > left) {
            _quickSortLoop(left, last, comp);
        }
    }
}

template<class RandomAccessIterator, class Compare>
void quickSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    _quickSortLoop(first, last - 1, comp);
}


template<class RandomAccessIterator>
void quickSort(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type T;
    _quickSortLoop(first, last - 1, std::less<T>());
}

}

#endif //ALG_QUICKSORT_H
