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

#include <flak/HashSet.h>
#include <hash_set>
#include <algorithm>
#include <cassert>
#include <iostream>
using namespace std;
using namespace flak;
using namespace __gnu_cxx;

void test1() {
    HashSet<int> iset;
    hash_set<int> sset;
    iset.insert(3);
    iset.insert(196);
    iset.insert(1);
    iset.insert(389);
    iset.insert(194);
    auto p = iset.insertNoresize(387);
    p.first.operator++();
    assert((p.first.cur_->val_ == 194));

    sset.insert(3);
    sset.insert(196);
    sset.insert(1);
    sset.insert(389);
    sset.insert(194);
    auto p2 = sset.insert_noresize(387);
    p2.first.operator++();
    assert((p2.first._M_cur->_M_val == 194));

    HashSet<int>::const_iterator it1 = iset.begin();
    auto it2 = iset.end();
    hash_set<int>::const_iterator its = sset.begin();
    for(; it1 != it2; it1++) {
        assert((*it1 == *its)); // [387 194 1] [389 196 3]
        ++its;
    }

    iset.insert(387);
    assert((iset.count(387) == 1));

    assert((iset.elemsInBucket(1) == sset.elems_in_bucket(1)));
    assert((iset.elemsInBucket(1) == 3)); // [387 194 1]

    assert((iset.maxBucketCount() == sset.max_bucket_count()));

    it1 = iset.begin();
    its = sset.begin();
    it1++, it1++, it1++, it1++;
    its++, its++, its++, its++;
    iset.erase(iset.begin(), it1);
    sset.erase(sset.begin(), its);

    it1 = iset.begin();
    it2 = iset.end();
    its = sset.begin();
    for(; it1 != it2; it1++) {
        assert((*it1 == *its)); // [196 3]
        ++its;
    }

    iset.erase(3);
    sset.erase(3);
    assert((iset.size() == 1));

    cout << "test 1 end" << endl;
}

int main() {
    test1();
}

