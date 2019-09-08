#include <map>
#include <set>
#include <iostream>
#include <cassert>
#include <flak/RBTree.h>
using namespace std;
using namespace flak;
void test1() {
    typedef RBTree<int, int, _Identity<int>, less<int> > TestRBTree;
    TestRBTree tree;
    cout << tree.size() << endl;
    assert((tree.size() == 0));

    tree.insertUnique(10);
    tree.insertUnique(7);
    tree.insertUnique(8);

    tree.insertUnique(15);
    tree.insertUnique(5);
    tree.insertUnique(6);
    tree.insertUnique(11);
    tree.insertUnique(13);
    tree.insertUnique(12);
    tree.insertUnique(12);

    cout << tree.size() << endl;
    assert((tree.size() == 9));

    TestRBTree::iterator it1 = tree.begin();
    TestRBTree::iterator it2 = tree.end();

    int ans[9] = {5,6,7,8,10,11,12,13,15};
    int cnt = 0;

    // pre-order traversal
    for(; it1 != it2; it1++) {
        cout << *it1 << " ";
        assert((*it1 == ans[cnt++]));
    }
    cout << endl;

    int color_ans[9] = {0,1,0,1,1,0,0,1,0};
    cnt = 0;
    it1 = tree.begin();
    for(; it1 != it2; it1++) {
        cout << *it1 << "(" << it1.node_->color_ << ")" << " ";
        assert((it1.node_->color_ == color_ans[cnt++]));
    }
    cout << endl;
    
    cout << "test RBTree end" << endl;
}

void test2() {


    typedef RBTree<double, double, _Identity<double>, less<double> > TestRBTree;
    TestRBTree tree;
    tree.insertUnique(10);
    tree.insertUnique(7);
    tree.insertUnique(8);

    tree.insertUnique(15);
    tree.insertUnique(5);
    tree.insertUnique(6);
    tree.insertUnique(11);
    tree.insertUnique(13);

//    tree.insertUnique(12);
//    tree.insertUnique(6.5);

    tree.erase(13);
    tree.erase(15);
    tree.erase(8);
    auto it = tree.find(7.5);
    cout << *it << endl;
}

void test3() {
    typedef _Rb_tree<double, double, _Identity<double>, less<double> > TestRBTree;
    TestRBTree tree;
    tree._M_insert_unique(10);
    tree._M_insert_unique(7);
    tree._M_insert_unique(8);

    tree._M_insert_unique(15);
    tree._M_insert_unique(5);
    tree._M_insert_unique(6);
    tree._M_insert_unique(11);
    tree._M_insert_unique(13);

//    tree.insertUnique(12);
//    tree.insertUnique(6.5);

    tree.erase(13);
    tree.erase(15);
    tree.erase(8);
    auto it = tree.find(7.5);
    cout << *it << endl;
}

int main() {
//    test1();
    test2();
}