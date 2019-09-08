#include "flak/alg/RandomShuffle.h"
#include "flak/alg/Reverse.h"
#include "flak/List.h"
#include "flak/alg/Search.h"
#include "flak/alg/Alg.h"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <vector>
using namespace flak;
using namespace std;

void testShuffle() {
    int a[5] = {4, 5, 6, 7, 8};
    randomShuffle(a, a + 5);
    std::copy(a, a + 5, std::ostream_iterator<int>(std::cout, " "));
    cout << endl;
}

void testReverse() {

    // RandomAccessIterator
    int a[5] = {4, 5, 6, 7, 8};
    int ans[5] = {8, 7, 6, 5, 4};
    Reverse(a, a + 5);
    assert((equal(a, a+5, ans, ans+5)));

    // BidirectionalIterator
    List<int> ls {0, 1, 1, 5, 6, 8, 3};
    List<int> ls2 {3, 8, 6, 5, 1, 1, 0};
    Reverse(ls.begin(), ls.end());
    assert((equal(ls.begin(), ls.end(), ls2.begin(), ls2.end())));
    cout << "test reverse end" << endl;
}

void testCount() {
    string s = "123ab3bcc33";
    long long n = count(s.begin(), s.end(), '3');
    assert((n == 4));
    char* sc = const_cast<char *>("123ab3bcc33");
    n = count(sc, sc + strlen(sc), '3');
    assert((n == 4));
    int ia[6] = {5, 3, 3, 2, 1, 6};
    n = count(ia, ia + 6, 3);
    assert((n == 2));

    List<string> ls {"hello", "c++", "java", "c++", "php", "python"};
    n = count_if(ls.begin(), ls.end(), [](string v) {
        return "hello " + v == "hello c++";
    });
    assert((n == 2));
    cout << "test count end" << endl;
}

void testSearch() {
    int arr[7] = {11, 15, 20, 21, 21, 23, 24};
    int* v = lowerBound(arr, arr + 7, 21);
    assert((*v == 21));
    int* v2 = upperBound(arr, arr + 7, 21);
    assert((*v2 == 23));
    int* v3 = upperBound(arr, arr + 7, 28);
    assert((v3 == arr + 7)); // last position

    bool isFind = binarySearch(arr, arr + 7, 21);
    assert(isFind);
    isFind = binarySearch(arr, arr + 7, 19);
    assert((isFind == false));

    // forward iterator
    List<int> ls = {11, 15, 20, 21, 21, 23, 24};
    auto its = lowerBound(ls.begin(), ls.end(), 21);
    auto it = ls.begin();
    advance(it, 3);
    assert((its == it));
    its = lowerBound(ls.begin(), ls.end(), 88);
    assert((its == ls.end()));

    int arr2[7] = {24, 23, 21, 21, 20, 15, 11};
    v = lowerBound(arr2, arr2 + 7, 21, greater<int>());
    assert((*v == 21));
    v2 = upperBound(arr2, arr2 + 7, 21, greater<int>());
    assert((*v2 == 20));

    cout << "test search end" << endl;
}

void testLexicographicalCompare() {
    string s = "abcde";
    string s2 = "abcd";
    bool b = lexicographicalCompare(s.begin(), s.end(), s2.begin(), s2.end());
    assert((b == false));

    s = "abcdefk";
    s2 = "abcdefg";
    b = lexicographicalCompare(s.begin(), s.end(), s2.begin(), s2.end());
    assert((b == false));

    s = "abcdefgh";
    s2 = "abcdefgv";
    b = lexicographicalCompare(s.begin(), s.end(), s2.begin(), s2.end());
    assert(b);

    cout << "test Lexicographical Compare end" << endl;
}

bool pred(int v) {
    return v % 2 == 0;
}

void testPartition() {
    vector<int> vec {0,1,2,3,4,4,5,5,6,6,6,6,6,7,8};
    flak::partition(vec.begin(), vec.end(), pred);
    vector<int> ans = {0,8,2,6,4,4,6,6,6,6,5,5,3,7,1};
    assert(equal(vec.begin(), vec.end(), ans.begin()));
    cout << "test Partition end" << endl;
}


int main() {
//    testShuffle();
//    testReverse();
//    testCount();
//    testSearch();
//    testLexicographicalCompare();

    for(int i = 0; i < 30; i++) {
        testPartition();
    }

}
