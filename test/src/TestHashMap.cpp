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

#include <flak/HashMap.h>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>

using namespace std;
using namespace flak;

void test1() {
    HashMap<string, int> hm;

    hm["Mary"] = 1;
    hm["John"] = 2;
    hm["Mike"] = 3;
    hm["Jack"] = 100;
    hm["James"] = 100;

    assert((hm.size() == 5));

    auto it1 = hm.begin();
    auto it2 = hm.end();
    for(; it1 != it2; it1++) {
        cout << it1->first << " " << it1->second << endl;
    }

    assert((hm.count("Mary") == 1));
    it1 = hm.begin();
    it1++, it1++;
    hm.erase(hm.begin(), it1);
    assert((hm.size() == 3));

    hm.insert(pair<string, int>("Liam", 33));
    hm.erase("John");
    it1 = hm.begin();
    it2 = hm.end();
    for(; it1 != it2; it1++) {
        cout << it1->first << " " << it1->second << endl;
    }

    const HashMap<string, int> hmc(hm.begin(), hm.end());
    HashMap<string, int>::const_iterator itc = hmc.begin();
    auto itc2 = hmc.end();
    for(; itc != itc2; itc++) {
        cout << itc->second << endl;
    }
    cout << "test 1 end";
}

int main() {
    test1();
}

