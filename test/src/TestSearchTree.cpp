#include <flak/SearchTree.h>
#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;
using namespace flak;
void test1() {
    typedef SearchTree<int, int, _Identity<int>, less<int>> STree;
    STree tree;
    assert((tree.empty()));

    tree.insertUnique(3);
    tree.insertUnique(6);
    tree.insertUnique(5);
    tree.insertUnique(4);
    tree.insertUnique(12);
    tree.insertUnique(1);
    tree.insertUnique(8);
    tree.insertUnique(2);

    assert((tree.size() == 8));

    int ans1[8] = {1,2,3,4,5,6,8,12};
    int cnt = 0;
    auto it1 = tree.begin();
    auto it2 = tree.end();
    for(; it1 != it2; it1++) {
        assert((*it1 == ans1[cnt++]));
    }

    int ans2[8] = {12,8,6,5,4,3,2,1};
    cnt = 0;
    it1 = tree.end();
    it2 = tree.begin();
    --it1;
    for(; it1 != it2; --it1) {
        assert((*it1 == ans2[cnt++]));
    }
    assert((*it1 == 1));

    it1 = tree.find(5);
    assert((*it1 == 5));

    tree.insertEqual(2);
    assert((tree.size() == 9));
    assert((tree.count(2) == 2));

    int ecnt = tree.erase(2);
    assert((ecnt == 2));
    assert((tree.size() == 7));

    ecnt = tree.erase(3);
    assert((ecnt == 1));
    assert((tree.size() == 6));

    ecnt = tree.erase(8);
    assert((ecnt == 1));
    assert((tree.size() == 5));

    ecnt = tree.erase(99);
    assert((ecnt == 0));
    assert((tree.size() == 5));

    cout << "test 1 end" << endl;
}

int main() {
    test1();
}