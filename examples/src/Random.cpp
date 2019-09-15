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


#include <flak/alg/RandomShuffle.h>
#include <flak/alg/RandomizedSelect.h>
#include <iostream>
#include <vector>
using namespace std;

void randomShuffle() {
    int a[5] = {4, 5, 6, 7, 8};
    flak::randomShuffle(a, a + 5);
    std::copy(a, a + 5, std::ostream_iterator<int>(std::cout, " "));
    cout << endl;
}

void randomizedSelect() {
    vector<int> vec{8, 12, 15, 10, 14, 23, 4, 16, 23, 6};
    vector<int>::iterator p = flak::randomizedSelect(vec.begin(), vec.end(), 5);
    cout << *p << endl; // 12
}

int main() {
    randomShuffle();
    randomizedSelect();
}

