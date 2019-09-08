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
#include <flak/Trie.h>
using namespace std;

int main() {
    typedef flak::Trie<string, int> Trie;
    Trie t;
    string s = "hello c++";
    string s2 = "hello java";
    string s3 = "bye php";
    string s4 = "hello javac";

    t.insert(s, 1);
    t.insert(s2, 3);
    t.insert(s3, 4);
    t.insert(s4, 8);

    // find a key
    string key = "";
    key = "hello java";
    pair<bool, Trie::iterator> res = t.find(key);
    if(res.first) {
        Trie::iterator it = res.second;
        cout << *it << endl; // 3
    }

    // find prefix
    string prefix = "hello";
    vector<Trie::iterator> ress = t.findPrefix(prefix);
    for(auto& p : ress) {
        cout << *p << " "; // 1 3 8
    }
    cout << endl;

    // erase prefix
    bool b = t.erasePrefix("hello");
    if(b) {
        cout << t.find(s).first << endl; // false
        cout << t.find(s2).first << endl; // false
        cout << t.find(s3).first << endl; // true
    }

}


