
#include <flak/alg/Sort.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace flak;
using std::cout;
using std::vector;
using std::endl;
using std::copy;

vector<int> randomSeq(int n) {
    vector<int> vec;
    vector<int> data;
    data.reserve(100);
    for(int i = 0; i < 100; i++) {
        data.push_back(i);
    }
    vec.reserve(n);
    for(int i = 0; i < n; i++) {
        vec.push_back(data[rand() % 100]);
    }
    return vec;
}

void testInsertionSort() {
    int arr[7] = {7, 4, 2, 5, 11, 8, 6};
    int ans[7] = {2,4,5,6,7,8,11};
    insertionSort(arr, arr+7);
    for(int i = 0; i < 7; i++) {
        assert((arr[i] == ans[i]));
    }
    cout << endl;

    std::reverse(ans, ans+7);
    insertionSort(arr, arr+7, std::greater<int>());
    for(int i = 0; i < 7; i++) {
        assert((arr[i] == ans[i]));
    }
    cout << endl;

    for(int i = 1; i < 100; i++) {
        auto v1 = randomSeq(i);
        vector<int> v2(v1);
        std::sort(v1.begin(), v1.end());
        insertionSort(v2.begin(), v2.end());
        assert((v1 == v2));
    }
    cout << "test insertion sort end" << endl;
}

void testQickSort() {
    int arr[7] = {7, 4, 2, 5, 11, 8, 6};
    int ans[7] = {2,4,5,6,7,8,11};
    quickSort(arr, arr+7);
    for(int i = 0; i < 7; i++) {
        assert((arr[i] == ans[i]));
    }
    cout << endl;

    std::reverse(ans, ans+7);
    quickSort(arr, arr+7, std::greater<int>());
    for(int i = 0; i < 7; i++) {
        assert((arr[i] == ans[i]));
    }
    cout << endl;

    for(int i = 1; i < 100; i++) {
        auto v1 = randomSeq(i);
        vector<int> v2(v1);
        std::sort(v1.begin(), v1.end());
        quickSort(v2.begin(), v2.end());
        assert((v1 == v2));
    }
    cout << "test quick sort end" << endl;
}

void testSort() {
    int arr[7] = {7, 4, 2, 5, 11, 8, 6};
    int ans[7] = {2,4,5,6,7,8,11};
    Sort(arr, arr+7);
    for(int i = 0; i < 7; i++) {
        assert((arr[i] == ans[i]));
    }
    cout << endl;

    std::reverse(ans, ans+7);
    Sort(arr, arr+7, std::greater<int>());
    for(int i = 0; i < 7; i++) {
        assert((arr[i] == ans[i]));
    }
    cout << endl;

    double stdsortUse = 0;
    double mySortUse = 0;
    for(int i = 1; i < 1000; i++) {
        auto v1 = randomSeq(i);
        vector<int> v2(v1);
        clock_t tStart = clock();
        std::sort(v1.begin(), v1.end());
        stdsortUse += double(clock() - tStart);

        tStart = clock();
        Sort(v2.begin(), v2.end());
        mySortUse += double(clock() - tStart);
        assert((v1 == v2));
    }

    cout << "Use Time: stdsort: " << stdsortUse << " mysort: " << mySortUse << endl;
    cout << "test sort end" << endl;
}

int main() {
    testInsertionSort();
    testQickSort();
    testSort();
}