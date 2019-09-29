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

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <vector>
#include <time.h>
#include <flak/alg/RandomizedSelect.h>
using namespace flak;
using namespace std;

void testRandomPartition() {
    vector<int> vec {8,12,15,10,14,23,4,16,23,6};
    auto it = randomPartition(vec.begin(), vec.end());
    cout << *it << endl;
    copy(vec.begin(), vec.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    cout << "test RandomPartition end" << endl;
}

void testRandomPartition2() {
    vector<int> vec {8,12,15,10,14,23,4,16,23,6};
    auto it = randomPartition(vec.begin(), vec.end(), std::greater<int>());
    cout << *it << endl;
    copy(vec.begin(), vec.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    cout << "test RandomPartition end" << endl;
}

void testRandomizedSelect() {
    vector<int> vec {8,12,15,10,14,23,4,16,23,6};

    auto p = randomizedSelect(vec.begin(), vec.end(), 5);
//    copy(vec.begin(), vec.end(), ostream_iterator<int>(cout, " "));
//    cout << "p:" << *p << endl;
    assert((*(p) == 12));

    p = randomizedSelect(vec.begin(), vec.end(), 2);
    assert((*(p) == 6));

    p = randomizedSelect(vec.begin(), vec.end(), 3);
    assert((*(p) == 8));

    p = randomizedSelect(vec.begin(), vec.end(), 8);
    assert((*(p) == 16));

    p = randomizedSelect(vec.begin(), vec.end(), 10);
    assert((*(p) == 23));
    p = randomizedSelect(vec.begin(), vec.end(), 9);
    assert((*(p) == 23));

    p = randomizedSelect(vec.begin(), vec.end(), 1);
    assert((*(p) == 4));
    cout << "test Randomized Select end" << endl;
}

void testRandomizedSelect2() {
    vector<int> vec {3,2,3,1,2,4,5,5,6};
    auto p = randomizedSelect(vec.begin(), vec.end(), 4);
    assert((*(p) == 3));
    cout << "test Randomized Select 2 end" << endl;
}

void testRandomizedSelect3() {
    vector<int> vec {1};
    auto p = randomizedSelect(vec.begin(), vec.end(), 1);
    assert((*(p) == 1));
    cout << "test Randomized Select 3 end" << endl;
}


int main() {
    testRandomPartition();
    testRandomPartition2();
    testRandomizedSelect();
    srand(time(0));
    for(int i = 0; i < 30; i++) {
        testRandomizedSelect();
    }
    for(int i = 0; i < 30; i++) {
        testRandomizedSelect2();
    }
    testRandomizedSelect3();
}
