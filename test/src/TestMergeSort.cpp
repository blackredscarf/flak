
#include <flak/alg/MergeSort.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
using namespace std;
using namespace flak;

vector<int> randomSeq(int n) {
    vector<int> vec;
    int data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for(int i = 0; i < n; i++) {
        vec.push_back(data[rand() % 10]);
    }
    return vec;
}

void testMergeSort() {
    int arr[7] = {7, 4, 2, 5, 11, 8, 6};
    int ans[7] = {2, 4, 5, 6, 7, 8, 11};
    mergeSort(arr, arr + 7);
    for (int i = 0; i < 7; i++) {
        assert((arr[i] == ans[i]));
    }
    cout << endl;

    std::reverse(ans, ans+7);
    mergeSort(arr, arr+7, std::greater<int>());
    for(int i = 0; i < 7; i++) {
        assert((arr[i] == ans[i]));
    }
    cout << endl;

    for(int i = 1; i < 100; i++) {
        auto v1 = randomSeq(i);
        vector<int> v2(v1);
        std::sort(v1.begin(), v1.end());
        mergeSort(v2.begin(), v2.end());
        assert((v1 == v2));
    }
    cout << "test merge sort end" << endl;
}

int main() {
    testMergeSort();
}