#include <flak/SList.h>
#include <cassert>
#include <iostream>
#include <algorithm>
using namespace std;
using namespace flak;

void test1() {
    SList<int> ls;
    assert((ls.size() == 0));

    ls.push_front(9);
    ls.push_front(1);
    ls.push_front(2);
    ls.push_front(3);
    ls.push_front(4);
    assert((ls.size() == 5));

    int ans[5] = {4,3,2,1,9};
    int cnt = 0;
    SList<int>::iterator it1 = ls.begin();
    SList<int>::iterator it2 = ls.end();
    for(; it1 != it2; ++it1) {
        assert((*it1 == ans[cnt++]));
    }

    it1 = find(ls.begin(), ls.end(), 1);
    assert((*it1 == 1));

    ls.insert_after(it1, 99);
    assert((ls.size() == 6));

    it1 = ls.begin();
    int ans2[6] = {4,3,2,1,99,9};
    cnt = 0;
    for(; it1 != it2; ++it1) {
        assert((*it1 == ans2[cnt++]));
    }

    it1 = find(ls.begin(), ls.end(), 3);
    assert((*it1 == 3));
    assert((*(ls.erase(it1)) == 2)); // erase

    it1 = ls.begin();
    int ans3[6] = {4,2,1,99,9};
    cnt = 0;
    for(; it1 != it2; ++it1) {
        assert((*it1 == ans3[cnt++]));
    }

    // previous
    assert((*ls.previous(ls.end()) == 9));

    // splice
    SList<int> ls2 {7,8,9};
    it1 = ls.begin();
    ls.splice_after(ls.begin(), ls2);
    int ans4[9] = {4,7,8,9,2,1,99,9};
    cnt = 0;
    for(; it1 != it2; ++it1) {
        assert((*it1 == ans4[cnt++]));
    }

    // splice 2
    int arr[5] = {22,33,44,55,66};
    SList<int> ls3(arr, arr + 3);
    assert((ls3.size() == 3));

    it1 = ls.begin();
    it1++, it1++, it1++;
    ls3.splice(ls3.begin(), ls, ls.begin(), it1);
    int ans5[6] = {4,7,8,22,33,44};
    cnt = 0;
    it1 = ls3.begin();
    it2 = ls3.end();
    for(; it1 != it2; ++it1) {
        assert((*it1 == ans5[cnt++]));
    }

    // clear
    ls.clear();
    assert((ls.size() == 0));

    // pop front and swap
    assert((ls3.front() == 4));
    ls3.pop_front();
    assert((*(ls3.begin()) == 7));

    SList<int> ls4 {};
    ls4.swap(ls3);
    assert((ls3.size() == 0));
    assert((ls4.size() == 5));

    cout << "test 1 end" << endl;
}

void test2() {
    SList<int> ls {100, 4, 2, 3, 6, 5, 18, 12, 31};
    int ans[] = {2,3,4,5,6,12,18,31,100};
    int cnt = 0;
    ls.sort();
    SList<int>::iterator it = ls.begin();
    for(; it != ls.end(); it++){
        auto v = *it;
        assert((v == ans[cnt++]));
    }
    cout << "test 2 end" << endl;
}

int main() {
    test1();
    test2();
}