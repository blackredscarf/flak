#include <flak/AVLTree.h>
#include <algorithm>
#include <iostream>
using namespace std;
using namespace flak;

void test1() {
    typedef AVLTree<int, int, _Identity<int>, less<int>> TestAVLTree;
    TestAVLTree tree;
    assert((tree.empty()));
    cout << tree.max_size() << endl;

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

    assert((tree.size() == 9));

    TestAVLTree::iterator it1 = tree.begin();
    TestAVLTree::iterator it2 = tree.end();

    int ans[9] = {5,6,7,8,10,11,12,13,15};
    int cnt = 0;
    for(; it1 != it2; it1++) {
        cout << *it1 << " ";
        assert((*it1 == ans[cnt++]));
    }
    cout << endl;
    cout << "test 1 end" << endl;
}

void test2() {
    typedef AVLTree<int, int, _Identity<int>, less<int>> TestAVLTree;
    TestAVLTree tree;

    tree.insertEqual(0);
    tree.insertEqual(1);
    tree.insertEqual(0);
    tree.insertEqual(2);
    tree.insertEqual(3);

    tree.insertUnique(6);
    tree.insertUnique(6);
    tree.insertUnique(7);
    tree.insertUnique(10);

    assert((tree.count(0) == 2));

    auto it = tree.find(7);
    assert((*it = 7));
    it = tree.find(4);
    assert((it == tree.end()));
    cout << "test 2 end" << endl;
}

void test3() {
    typedef AVLTree<int, int, _Identity<int>, less<int>> TestAVLTree;
    TestAVLTree tree;

    tree.insertEqual(0);
    tree.insertEqual(1);
    tree.insertEqual(0);
    tree.insertEqual(2);
    tree.insertEqual(3);

    tree.insertUnique(6);
    tree.insertUnique(6);
    tree.insertUnique(7);
    tree.insertUnique(10);

    tree.erase(6);
    assert((tree.size() == 7));
    tree.erase(0);
    assert((tree.size() == 5));

    auto it = tree.find(7);
    assert((*it = 7));

    tree.erase(1);
    assert((tree.size() == 4));
    tree.erase(2);
    assert((tree.size() == 3));

    it = tree.find(7);
    assert((*it = 7));

    tree.erase(7);
    assert((tree.size() == 2));
    tree.erase(10);
    assert((tree.size() == 1));

    it = tree.find(3);
    assert((*it = 3));

    tree.erase(3);
    assert((tree.size() == 0));

    cout << "test 3 end" << endl;
}

void test4() {
    typedef AVLTree<int, int, _Identity<int>, less<int>> TestAVLTree;
    TestAVLTree tree;
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

    TestAVLTree::iterator it1 = tree.begin();
    TestAVLTree::iterator it2 = tree.end();

    for(; it1 != it2; it1++) {
        cout << *it1 << " left: ";
        if(it1.node_->left_ != nullptr) {
            cout << it1.node_->left_->value_;
        }
        cout << " right: ";
        if(it1.node_->right_ != nullptr) {
            cout << it1.node_->right_->value_;
        }
        cout << endl;
    }
    cout << endl;
    /*
     *             8
     *           /   \
     *          /     \
     *         6      11
     *        / \    / \
     *       5   7  10  13
     *                 / \
     *                12  15
     *
     */
    cout << "test 4 end" << endl;
}

void test5() {
    typedef AVLTree<int, int, _Identity<int>, less<int>> TestAVLTree;
    TestAVLTree tree;
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

    TestAVLTree::iterator it1 = tree.end();
    TestAVLTree::iterator it2 = tree.begin();

    it1--;
    int ans[9] = {15,13,12,11,10,8,7,6,5};
    int cnt = 0;
    for(; it1 != it2; --it1) {
        assert((*it1 == ans[cnt++]));
    }
    assert((*it1 == 5));

    cout << endl;
}

int main() {
//    test1();
//    test2();
//    test3();
//    test4();
    test5();
}