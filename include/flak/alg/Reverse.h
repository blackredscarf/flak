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

#ifndef ALG_REVERSE_H
#define ALG_REVERSE_H

#include <iterator>
using std::__iterator_category;
using std::bidirectional_iterator_tag;
using std::random_access_iterator_tag;
using std::iter_swap;

namespace flak {

template<class BidirectionalIterator>
void _Reverse(BidirectionalIterator first,
              BidirectionalIterator last, bidirectional_iterator_tag) {
    while (true) {
        if (first == last || first == --last) {
            return;
        } else {
            iter_swap(first++, last);
        }
    }
}

template<class RandomAccessIterator>
void _Reverse(RandomAccessIterator first,
              RandomAccessIterator last, random_access_iterator_tag) {
    while (first < last) { // only random have operator< for two iterator
        iter_swap(first++, --last);
    }
}

template<class BidirectionalIterator>
void Reverse(BidirectionalIterator first, BidirectionalIterator last) {
    _Reverse(first, last, __iterator_category(first));
}

}
#endif //ALG_REVERSE_H
