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

#ifndef ALG_ALG_H
#define ALG_ALG_H

#include <iterator>
#include <list>
#include <functional>
using std::iterator_traits;

namespace flak {
template<typename I, typename T>
typename iterator_traits<I>::difference_type
Count(I first, I last, const T &value) {
    typename iterator_traits<I>::difference_type n = 0;
    for (; first != last; ++first) {
        if (*first == value) {
            ++n;
        }
    }
    return n;
}

template<typename I, typename Pred>
typename iterator_traits<I>::difference_type
CountIf(I first, I last, Pred pred) {
    typename iterator_traits<I>::difference_type n = 0;
    for (; first != last; ++first) {
        if (pred(*first)) {
            ++n;
        }
    }
    return n;
}

template <class BidirectionalIterator, class Predicate>
BidirectionalIterator partition(BidirectionalIterator first,
        BidirectionalIterator last, Predicate pred) {
    while (true) {
        while (true) {
            if(first == last) {
                return first;
            } else if(pred(*first)) {
                ++first;
            } else {
                break;
            }
        }
        --last;
        while (true) {
            if(first == last) {
                return first;
            } else if(!pred(*last)) {
                --last;
            } else {
                break;
            }
        }
        std::iter_swap(first, last);
        ++first;
    }
}

template<typename InputIterator1, typename InputIterator2, typename Compare>
bool lexicographicalCompare(InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, InputIterator2 last2, Compare comp) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (comp(*first1, *first2)) {
            return true;
        }
        if (comp(*first2, *first1)) {
            return false;
        }
    }
    return first1 == last1 && first2 != last2;
}

template<typename InputIterator1, typename InputIterator2>
bool lexicographicalCompare(InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, InputIterator2 last2) {
    typedef typename iterator_traits<InputIterator1>::value_type T;
    return lexicographicalCompare(first1, last1, first2, last2, std::less<T>());
}

}
#endif //ALG_ALG_H
