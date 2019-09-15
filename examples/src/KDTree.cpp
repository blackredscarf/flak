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
#include <vector>
#include <string>
#include <flak/KDTree.h>
using namespace std;

int main() {
    vector<int> vec1 {4,4};
    vector<int> vec2 {2,3};
    vector<int> vec3 {5,6};
    vector<int> vec4 {7,9};
    vector<int> vec5 {3,1};

    // specify point type and value type
    flak::KDTree<vector<int>, int> kd(2); // 2d array
    kd.insert(vec1, 1);
    kd.insert(vec2, 2);
    kd.insert(vec3, 3);
    kd.insert(vec4, 4);
    kd.insert(vec5, 5);

    typedef flak::KDTree<vector<int>, int>::iterator kditer;

    // find a point
    pair<bool, kditer> i1 = kd.find(vec1);
    if(i1.first) { // the point is in the tree
        kditer it1 = i1.second; // the value of kd tree iterator is a pair
        vector<int> point = it1->first;
        int value = it1->second;
        cout << value << endl; // 1
    }

    // find the k nearest points
    vector<pair<size_t, kditer>> ks = kd.findKNearest(vec2, 2);
    for(auto d : ks) {
        cout << "distance: " << d.first << " ";
        cout << "value: " << d.second->second << endl;
    }
    cout << endl;

    // remove a point
    kd.erase(vec3);
    ks = kd.findKNearest(vec2, 2);
    for(auto d : ks) {
        cout << "distance: " << d.first << " ";
        cout << "value: " << d.second->second << endl;
    }
}



