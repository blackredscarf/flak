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


#include <flak/AVLSet.h>    // avl tree set
#include <flak/HashSet.h>   // hash set
#include <flak/Set.h>       // red black tree set
#include <iostream>
using namespace std;

void interface() {
    flak::AVLSet<string, int> avlset;
    flak::HashSet<string, int> hashset;
    flak::Set<string, int> sset;
}

int main() {
    int ia[5] = {0, 1, 2, 3, 4};
    flak::AVLSet<int> iset(ia, ia+5);
    cout << "size: " << iset.size() << endl;

    // insert
    iset.insert(11);
    cout << "size: " << iset.size() << endl;

    // count
    cout << "count: " << iset.count(3) << endl;

    // erase
    iset.erase(1);
    auto it = iset.find(1);
    cout << (it == iset.end()) << endl;

    // iterator
    auto it1 = iset.begin();
    for(; it1 != iset.end(); it1++) {
        cout << *it1 << " ";
    }
    cout << endl;
}