
#include <flak/AVLMap.h>
#include <map>
#include <iostream>
#include <cassert>
using namespace std;
using namespace flak;

void test1() {
    AVLMap<string, int> smap;
    smap["jjhou"] = 1;
    smap["jerry"] = 2;
    smap["jason"] = 3;
    smap["jimmy"] = 4;
    assert((smap.size() == 4));

    smap.erase("jason");
    smap.insert(pair<string, int>("david", 5));

    AVLMap<string, int>::iterator it1 = smap.begin();
    assert((smap.size() == 4));
    string ansk[5] = {"david","jerry","jimmy","jjhou"};
    int ansv[5] = {5,2,4,1};
    int cnt = 0;
    for(; it1 != smap.end(); it1++) {
        assert((it1->first == ansk[cnt] && it1->second == ansv[cnt++]));
    }

    int number = smap["jjhou"];
    assert((number == 1));

    it1 = smap.find("mchen");
    assert((it1 == smap.end()));

    it1 = smap.find("jerry");
    assert((it1->second == 2));


    it1->second = 9;
    // it1->first = "ss"; // cannot update the key
    assert((smap["jerry"] == 9));
    cout << "test 1 end" << endl;
}

void test2() {
    AVLMap<string, int> smap;
    smap["jjhou"] = 1;
    smap["jerry"] = 2;
    smap["jason"] = 3;
    smap["Bob"] = 5;
    smap["Mike"] = 6;
    smap["James"] = 7;
    smap["John"] = 8;
    assert((smap.size() == 7));

    smap.erase("jason");
    assert((smap.find("jason") == smap.end()));
    assert((smap.size() == 6));

    smap["jimmy"] = 4;
    assert((smap.size() == 7));

    smap.erase("jerry");
    assert((smap.find("jerry") == smap.end()));
    assert((smap.size() == 6));

    smap.erase("Bob");
    assert((smap.find("Bob") == smap.end()));
    assert((smap.size() == 5));

    smap.erase("John");
    assert((smap.find("John") == smap.end()));
    assert((smap.size() == 4));

    smap.erase("jjhou");
    assert((smap.find("jjhou") == smap.end()));
    assert((smap.size() == 3));

    assert((smap.find("jimmy")->second == 4));

    smap.erase("jimmy");
    assert((smap.find("jimmy") == smap.end()));
    assert((smap.size() == 2));

    smap.erase("James");
    assert((smap.find("James") == smap.end()));
    assert((smap.size() == 1));

    smap.erase("Mike");
    assert((smap.find("Mike") == smap.end()));
    assert((smap.empty()));

    cout << "test 2 end" << endl;
}

void test3() {
    AVLMap<string, int> smap;
    smap["jjhou"] = 1;
    smap["jerry"] = 2;
    smap["jason"] = 3;
    smap["jimmy"] = 4;

    auto it = smap.begin();
    string s = it->first;
    it++, it++;
    smap.erase(smap.begin(), it);
    assert((smap.find(s) == smap.end()));
    assert((smap.size() == 2));

    cout << "test 3 end" << endl;
}

int main() {
    test1();
    test2();
    test3();
}

