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

#ifndef ALG_RANDOMSHUFFLE_H
#define ALG_RANDOMSHUFFLE_H
#include <iterator>
using std::iter_swap;

namespace flak {


// Fisher–Yates shuffle Random shuffle
// Refer The Art of Computer Programming 3.4.2 Algorithm P.
// The different is that we make the swap left to right,
// the book's is right to left.
// 0 1 2 3 4 5
// [0 1] 2 3 4 5 -> [1 0] 2 3 4 5 : 1 and 0 swap, swap distance is 2
// [1 0 2] 3 4 5 -> [2 0 1] 3 4 5 : 2 and 1 swap, swap distance is 3
// [2 0 1 3] 4 5 -> [2 0 3 1] 4 5 : 3 and 1 swap, swap distance is 4
// [2 0 3 1 4] 5 -> [2 4 3 1 0] 5 : 4 and 0 swap, swap distance is 5
// [2 4 3 1 0 5] -> [2 4 3 1 0 5] : 5 and 5 swap, swap distance is 6
template<class RandomAcessIterator>
inline void randomShuffle(RandomAcessIterator first, RandomAcessIterator last) {
    if (first == last) return;
    for (RandomAcessIterator i = first + 1; i != last; i++) {
        iter_swap(i, first + rand() % ((i - first) + 1));
    }
}

}
#endif //ALG_RANDOMSHUFFLE_H
