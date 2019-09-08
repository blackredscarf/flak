#include <flak/Map.h>
#include <map>
#include <iostream>
#include <cassert>
using namespace std;
using namespace flak;

int main() {
    map<string, int> m;
    m["hello"] = 1;

    Map<string, int> smap;
    smap["jjhou"] = 1;
    smap["jerry"] = 2;
    smap["jason"] = 3;
    smap["jimmy"] = 4;
    assert((smap.size() == 4));

    smap.insert(pair<string, int>("david", 5));

    Map<string, int>::iterator it1 = smap.begin();
    smap.erase("jason");
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
    cout << "end" << endl;
}
