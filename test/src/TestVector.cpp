#include <flak/Vector.h>
#include <vector>
#include <cassert>
#include <iostream>
using namespace std;
using namespace flak;

void test1() {
    int a[7] = {0,6,7,9,10,4,4};
    Vector<int> v1(a, a+7);
    vector<int> v2(a, a+7);
    assert((v1.size() == v2.size()));
    assert((v1.size() == 7));
    assert((v1.front() == v2.front()));

    v1.pop_back();
    v2.pop_back();
    cout << v1.size() << endl;
    assert((v1.size() == v2.size()));

    for(int i = 0; i < v1.size(); i++) {
        assert((v1[i] == v2[i]));
    }

    v1.push_back(33);
    v2.push_back(33);
    cout << v1.size() << endl;
    assert((v1.size() == v2.size()));
    assert((v1.back() == v2.back()));

    v1.erase(v1.begin() + 2, v1.end() - 1);
    v2.erase(v2.begin() + 2, v2.end() - 1);
    assert((v1.front() == v2.front()));
    assert((v1.back() == v2.back()));

    v1.erase(v1.begin());
    v2.erase(v2.begin());
    assert((v1.front() == v2.front()));
    assert((v1.size() == v2.size()));
    cout << "test 1 end" << endl;
}

void test2() {
    Vector<int> v1{1};
    vector<int> v2{1};
    assert((v1.size() == v2.size()));
    cout << v2.capacity() << endl;
    assert((v1.capacity() == v2.capacity()));

    v1.push_back(2);
    v1.push_back(3);
    v2.push_back(2);
    v2.push_back(3);
    assert((v1.capacity() == v2.capacity()));
    assert((v1.capacity() == 4));

    v1.reserve(32);
    v2.reserve(32);
    assert((v1.capacity() == 32));
    assert((v1.capacity() == v2.capacity()));
    cout << "test 2 end" << endl;
}

void test3() {
    Vector<int> v1{1,2,3,4};
    Vector<int> v1copy;
    assert((v1copy.empty()));
    v1copy = v1;

    vector<int> v2{1,2,3,4};
    vector<int> v2copy;
    v2copy = v2;

    cout << v1copy.size() << endl;
    assert((v1copy.size() == v2copy.size()));
    assert((v1.capacity() == v2.capacity()));

    for(int i = 0; i < v1copy.size(); i++) {
        assert((v1copy[i] == v2copy[i]));
    }
    cout << "test 3 end" << endl;
}

void test4() {
    Vector<int> v1{1,2,3,4};
    Vector<int> v1copy(v1);

    vector<int> v2{1,2,3,4};
    vector<int> v2copy(v2);

    assert((v1copy.size() == v2copy.size()));
    assert((v1.capacity() == v2.capacity()));

    for(int i = 0; i < v1copy.size(); i++) {
        assert((v1copy[i] == v2copy[i]));
    }
    cout << "test 4 end" << endl;
}

void test5() {
    Vector<int> v1(5, 222);
    int ans[5] = {222,222,222,222,222};
    assert((v1.size() == 5));
    assert((v1.capacity() == 5));
    for(int i = 0; i < v1.size(); i++) {
        assert((v1[i] == ans[i]));
    }
    cout << "test 5 end" << endl;
}

void test6() {
    Vector<int> v1;
    v1.reserve(5);
    Vector<int> v2{1,2,3};
    v1 = v2;
    assert((v1.size() == 3));
    cout << v1.size() << endl;
    assert((v1.capacity() == 5));
    assert((v1.front() == 1 && v1.back() == 3));

    v1.clear();
    assert((v1.size() == 0));
    assert((v2.front() == 1));

    Vector<double> v3(4);
    assert((v3.capacity() == 4));
    cout << "test 6 end" << endl;
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
}