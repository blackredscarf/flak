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
#include <flak/Trie.h>
using namespace std;
using namespace flak;

void test1() {
    typedef Trie<string, int> Trie;
    Trie t;
    string s = "hello c++";
    string s2 = "hello java";
    t.insert(s.begin(), s.end(), 1);
    t.insert(s2.begin(), s2.end(), 3);

    string prefix = "";

    prefix = "hello java";
    pair<bool, Trie::iterator> res = t.find(prefix.begin(), prefix.end());
    assert(res.first);
    Trie::iterator it = res.second;
    assert((*it == 3));

    cout << "test 1 endl" << endl;
}

void test2() {
    typedef Trie<string, int> Trie;
    Trie t;
    string s = "hello c++";
    string s2 = "hello java";
    t.insert(s, 1);
    t.insert(s2, 3);

    string prefix = "";

    prefix = "hello java";
    pair<bool, Trie::iterator> res = t.find(prefix);
    assert(res.first);
    Trie::iterator it = res.second;
    assert((*it == 3));

    cout << "test 2 endl" << endl;
}

void test3() {
    typedef Trie<string, int> Trie;
    Trie t;
    string s = "hello c++";
    string s2 = "hello java";
    string s3 = "bye php";
    string s4 = "hello javac";

    t.insert(s, 1);
    t.insert(s2, 3);
    t.insert(s3, 4);
    t.insert(s4, 8);

    string prefix = "";
    prefix = "hello";
    vector<Trie::iterator> res = t.findPrefix(prefix);
    assert((res.size() == 3));

    int ans[3] = {1, 3, 8}, cnt = 0;
    for(auto& p : res) {
        assert((*p == ans[cnt++]));
    }

    cout << "test 3 endl" << endl;
}

void test4() {
    typedef Trie<int*, string> Trie;
    Trie t;
    int s[8] = {0,1,2,3,4,5,6,7};
    int s2[6] = {0,1,2,88,4,5};
    int s3[6] = {0,1,3,10,12,14};
    int s4[5] = {0,1,3,6,3};

    t.insert(s, s+8, "data1");
    t.insert(s2, s2+6, "data2");
    t.insert(s3, s3+6, "data3");
    t.insert(s4, s4+5, "data4");

    int prefix[3] = {0,1,2};
    vector<Trie::iterator> res = t.findPrefix(prefix, prefix + 3);
    assert((res.size() == 2));

    string ans[2] = {"data1", "data2"}; int cnt = 0;
    for(auto& p : res) {
        assert((*p == ans[cnt++]));
    }

    cout << "test 4 endl" << endl;
}

void test5() {
    typedef Trie<string, int> Trie;
    Trie t;
    string s = "hello c++";
    string s2 = "hello java";
    string s3 = "bye php";
    string s4 = "hello javac";

    t.insert(s, 1);
    t.insert(s2, 3);
    t.insert(s3, 4);
    t.insert(s4, 8);

    bool b = t.erase(s3);
    assert(b);
    b = t.erase("hello");
    assert(!b);

    auto res = t.find(s3);
    assert(!res.first);
    res = t.find(s4);
    assert(res.first);
    assert((*(res.second) = 8));

    cout << "test 5 endl" << endl;
}

void test6() {
    typedef Trie<string, int> Trie;
    Trie t;
    string s = "hello c++";
    string s2 = "hello java";
    string s3 = "bye php";
    string s4 = "hello javac";
    string s5 = "hello";
    string s6 = "hell";

    t.insert(s, 1);
    t.insert(s2, 3);
    t.insert(s3, 4);
    t.insert(s4, 8);
    t.insert(s5, 12);
    t.insert(s6, 16);

    bool b = t.erasePrefix("hello");
    assert(b);

    assert(!t.find(s).first);
    assert(!t.find(s2).first);
    assert(!t.find(s4).first);
    assert(!t.find(s5).first);

    assert(t.find(s3).first);
    assert(t.find(s6).first);
    assert((t.existPrefix("hell").second == 1));

    cout << "test 6 endl" << endl;
}

void test7() {
    typedef Trie<string, int> Trie;
    Trie t;
    string s = "hello c++";
    string s2 = "hello java";
    string s3 = "bye php";
    string s4 = "hello javac";
    string s5 = "hello";
    string s6 = "hell";

    t.insert(s, 1);
    t.insert(s2, 3);
    t.insert(s3, 4);
    t.insert(s4, 8);
    t.insert(s5, 12);
    t.insert(s6, 16);

    assert(t.existPrefix("he").first);
    assert((t.existPrefix("he").second == 5));
    assert((t.existPrefix("bye").first && t.existPrefix("bye").second == 1));
    assert((t.existPrefix("hell").first && t.existPrefix("hell").second == 5));
    assert((t.existPrefix("hello ja").first && t.existPrefix("hello ja").second == 2));
    assert((t.existPrefix("hello c++").first && t.existPrefix("hello c++").second == 1));

    assert(!t.existPrefix("heo").first);
    assert(!t.existPrefix("hello jab").first);
    assert(!t.existPrefix("hello cpp").first);

    cout << "test 7 endl" << endl;
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
}


