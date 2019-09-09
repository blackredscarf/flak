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

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <flak/KDTree.h>
using namespace std;
using namespace flak;

void test1() {
    vector<int> vec1 {1,2};
    vector<int> vec2 {2,3};
    vector<int> vec3 {5,6};
    KDTree<vector<int>, int> kd(2);
    kd.insert(vec1, 1);
    kd.insert(vec2, 2);
    kd.insert(vec3, 3);


    typedef KDTree<vector<int>, int>::iterator kditer;
    vector<int> p{2,2};
    pair<bool, kditer> res = kd.find(p);
    assert(!res.first);

    int ans[2][2] = {{25, 3}, {1, 1}}, cnt = 0;
    kd.erase(vec2);
    vector<pair<size_t, kditer>> ks = kd.findKNearest(p, 2);
    for(auto d : ks) {
        assert((d.first == ans[cnt][0]));
        assert((d.second->second == ans[cnt++][1]));
//        cout << "distance: " << d.first << " ";
//        cout << "value: " << d.second->second << endl;
    }
    cout << "test 1 end" << endl;
}

void test2() {
    vector<int> vec1 {5,6,9,9};
    vector<int> vec2 {1,2,3,5};
    vector<int> vec3 {2,3,4,6};
    vector<int> vec4 {7,6,9,4};
    vector<int> vec5 {9,3,9,9};
    KDTree<vector<int>, int> kd(4);
    kd.insert(vec1, 1);
    kd.insert(vec2, 2);
    kd.insert(vec3, 3);
    kd.insert(vec4, 4);
    kd.insert(vec5, 5);

    cout << "vec1" << endl;
    auto i1 = kd.find(vec1);
    cout << i1.second.node_->value_.second << endl;
    cout << "dim:" << i1.second.node_->dim_ << endl;
    if(i1.second.node_->low_ != nullptr)
        cout << "low:" << i1.second.node_->low_->value_.second << endl;
    if(i1.second.node_->high_ != nullptr)
        cout << "high:" <<  i1.second.node_->high_->value_.second << endl;

    cout << "vec2" << endl;
    auto i2 = kd.find(vec2);
    cout << i2.second.node_->value_.second << endl;
    cout << "dim:" << i2.second.node_->dim_ << endl;
    if(i2.second.node_->low_ != nullptr)
    cout << "low:" <<  i2.second.node_->low_->value_.second << endl;
    if(i2.second.node_->high_ != nullptr)
    cout << "high:" <<  i2.second.node_->high_->value_.second << endl;

    cout << "vec3" << endl;
    auto i3 = kd.find(vec3);
    cout << i3.second.node_->value_.second << endl;
    cout << "dim:" << i3.second.node_->dim_ << endl;
    if(i3.second.node_->low_ != nullptr)
    cout << "low:" <<  i3.second.node_->low_->value_.second << endl;
    if(i3.second.node_->high_ != nullptr)
    cout << "high:" <<  i3.second.node_->high_->value_.second << endl;

    cout << "vec4" << endl;
    auto i4 = kd.find(vec4);
    cout << i4.second.node_->value_.second << endl;
    cout << "dim:" << i4.second.node_->dim_ << endl;
    if(i4.second.node_->low_ != nullptr)
    cout << "low:" << i4.second.node_->low_->value_.second << endl;
    if(i4.second.node_->high_ != nullptr)
    cout << "high:" <<  i4.second.node_->high_->value_.second << endl;

    cout << "vec5" << endl;
    auto i5 = kd.find(vec5);
    cout << i5.second.node_->value_.second << endl;
    cout << "dim:" << i5.second.node_->dim_ << endl;
    if(i5.second.node_->low_ != nullptr)
    cout << "low:" <<  i5.second.node_->low_->value_.second << endl;
    if(i5.second.node_->high_ != nullptr)
    cout << "high:" << i5.second.node_->high_->value_.second << endl;

    kd.erase(vec1);

    auto n4 = kd.find(vec4);
    cout << n4.second.node_->dim_ << endl;

    cout << "test 2 end" << endl;
}

void test3() {
    vector<int> vec1 {4,4};
    vector<int> vec2 {2,3};
    vector<int> vec3 {5,6};
    vector<int> vec4 {7,9};
    vector<int> vec5 {3,1};
    KDTree<vector<int>, int> kd(2);
    kd.insert(vec1, 1);
    kd.insert(vec2, 2);
    kd.insert(vec3, 3);
    kd.insert(vec4, 4);
    kd.insert(vec5, 5);

    typedef KDTree<vector<int>, int>::iterator kditer;
    vector<int> p{2,3};
    pair<bool, kditer> res = kd.find(p);
    if(res.first) {
        assert((res.second->second == 2));
        cout << "value: " << res.second->second << endl;
    }

    vector<pair<size_t, kditer>> ks2 = kd.findKNearest(vec1, 2);
    int ans[2][2] = {{5, 3}, {5, 2}};
    int cnt = 0;
    for(auto d : ks2) {
        assert((d.first == ans[cnt][0]));
        assert((d.second->second == ans[cnt++][1]));
//        cout << "distance: " << d.first << " ";
//        cout << "value: " << d.second->second << endl;
    }
    cout << endl;

    int ans2[2][2] = {{5, 5}, {5, 1}};
    int cnt2 = 0;
    kd.erase(vec2);
    vector<pair<size_t, kditer>> ks = kd.findKNearest(p, 2);
    for(auto d : ks) {
        assert((d.first == ans2[cnt2][0]));
        assert((d.second->second == ans2[cnt2++][1]));
//        cout << "distance: " << d.first << " ";
//        cout << "value: " << d.second->second << endl;
    }
    cout << endl;


    int ans3[2][2] = {{5, 5}, {5, 1}};
    int cnt3 = 0;
    ks2 = kd.findKNearest(vec1, 2);
    for(auto d : ks2) {
        assert((d.first == ans2[cnt3][0]));
        assert((d.second->second == ans2[cnt3++][1]));
//        cout << "distance: " << d.first << " ";
//        cout << "value: " << d.second->second << endl;
    }

    cout << "test 3 end" << endl;
}

int main() {
    test1();
//    test2();
    test3();
}