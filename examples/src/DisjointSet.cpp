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


#include <flak/graph/DisjointSet.h>
#include <iostream>
#include <string>
using namespace std;

int main() {
    int data[5] = {1, 2, 3, 4, 5};
    flak::DisjointSet<int> dset(data, data + 5);
    dset.insert(6);
    
    bool res = dset.insert(6);
    cout << res << endl; // false

    dset.merge(2, 3);
    dset.merge(4, 5);
    dset.merge(1, 6);

    cout << dset.find(2).second << endl; // 2
    cout << dset.find(5).second << endl; // 4
    cout << dset.find(6).second << endl; // 1
}
