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

#include<flak/AVLMap.h>     // avl tree map
#include <flak/HashMap.h>   // hash map
#include <flak/Map.h>       // red black tree map
#include <iostream>
using namespace std;

void interface() {
    flak::AVLMap<string, int> avlmap;
    flak::HashMap<string, int> hashmap;
    flak::Map<string, int> rbmap;
}

int main() {
    // The interface is the same as the STL map
    flak::AVLMap<string, int> smap;
    smap["jjhou"] = 1;
    smap["jerry"] = 2;
    smap["jason"] = 3;
    smap["jimmy"] = 4;

    flak::AVLMap<string, int>::iterator it1 = smap.begin();
    for(; it1 != smap.end(); it1++) {
        cout << it1->first << " : " << it1->second << endl;
    }

    smap.erase("jason");
    it1 = smap.find("jason");
    cout << (it1 == smap.end()) << endl; // true
}
