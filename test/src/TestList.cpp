#include <flak/List.h>
#include <iostream>
#include <list>
#include <ctime>
#include <cassert>
#include <sstream>
#include <algorithm>
using namespace std;
using namespace flak;

void test() {
    List<int> ls;
    ls.push_back(1);
    ls.push_back(2);
    ls.push_back(3);
    ls.push_front(0);
    ls.push_front(-1);

    ls.pop_back();
    ls.pop_front();
    ls.remove(2);

    assert((ls.size() == 2));
    assert((ls == List<int>{0, 1}));

    List<int> ls2 {4, 5, 6};
    ls.splice(ls.begin(), ls2);
    assert((ls2.size() == 0 && ls2.begin() == ls2.end()));
    assert((ls == List<int>{4, 5, 6, 0, 1}));

    ls.reverse();
    assert((ls == List<int>{1, 0, 6, 5, 4}));

    assert((ls.front() == 1));
    assert((ls.back() == 4));

    ostringstream os;
    List<int>::iterator it = ls.begin();
    for(; it != ls.end(); it++){
        auto v = *it;
        os << v << " ";
    }
    assert((os.str() == "1 0 6 5 4 "));

    List<int> ls3 {55, 66, 77, 88, 99};
    auto it3 = ls3.begin();
    it3++, it3++, it3++;
    ls.splice(ls.begin(), ls3, ls3.begin(), it3);
    assert((ls == List<int>{55, 66, 77, 1, 0, 6, 5, 4}));
    assert((ls3 == List<int>{88, 99}));

    int* indata = new int[3] {12, 13, 14};
    ls3.insert(ls3.begin(), indata, indata + 3);
    assert((ls3 == List<int>{12, 13, 14, 88, 99}));

    ls.clear();
    assert((ls.empty() == true));
    assert((ls.begin() == ls.end()));
}

void test2() {
    list<int> ls;
    ls.push_back(1);
    ls.push_back(2);
    ls.push_back(3);
    ls.push_front(0);
    ls.push_front(-1);
    ls.pop_back();
    ls.pop_front();
    ls.remove(2);

    assert((ls == list<int>{0, 1}));

    list<int> ls2 {4, 5, 6};
    ls.splice(ls.begin(), ls2);
    assert((ls == list<int>{4, 5, 6, 0, 1}));

    ls.reverse();
    assert((ls == list<int>{1, 0, 6, 5, 4}));

    assert((ls.front() == 1));
    assert((ls.back() == 4));

    ostringstream os;
    list<int>::iterator it = ls.begin();
    for(; it != ls.end(); it++){
        auto v = *it;
        os << v << " ";
    }
    assert((os.str() == "1 0 6 5 4 "));

    list<int> ls3 {55, 66, 77, 88, 99};
    auto it3 = ls3.begin();
    it3++, it3++, it3++;
    ls.splice(ls.begin(), ls3, ls3.begin(), it3);
    assert((ls == list<int>{55, 66, 77, 1, 0, 6, 5, 4}));
    assert((ls3 == list<int>{88, 99}));

    int* indata = new int[3] {12, 13, 14};
    ls3.insert(ls3.begin(), indata, indata + 3);
    assert((ls3 == list<int>{12, 13, 14, 88, 99}));

    ls.clear();
    assert((ls.empty() == true));
    assert((ls.begin() == ls.end()));
}

void test3() {
    List<int> ls2 {4, 5, 6};
    auto it = find(ls2.begin(), ls2.end(), 6);
    cout << *it << endl;
    assert((*it == 6));
}

void test4() {
    List<int> ls {100, 4, 2, 3, 6, 5, 18, 12, 31};
    int ans[] = {2,3,4,5,6,12,18,31,100};
    int cnt = 0;
    ls.sort();
    List<int>::iterator it = ls.begin();
    for(; it != ls.end(); it++){
        auto v = *it;
        assert((v == ans[cnt++]));
    }
    cout << "test 4 end" << endl;
}


void testTime() {
    clock_t tStart = clock();
    for(int i = 0; i < 10000; i++) {
        test();
    }
    cout << (double) (clock() - tStart) << endl;

    tStart = clock();
    for(int i = 0; i < 10000; i++) {
        test2();
    }
    cout << (double) (clock() - tStart) << endl;
}

int main() {
//    testTime();
    test4();
}