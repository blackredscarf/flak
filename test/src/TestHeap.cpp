#include <flak/Heap.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;
using namespace flak;

int main() {
    vector<int> vec{0,1,2,3,4,8,9,3,5};

    makeHeap(vec.begin(), vec.end());
    int ans[9] = {9,5,8,3,4,0,2,3,1};
    for(int i = 0; i < vec.size(); i++) {
        assert((vec[i] == ans[i]));
    }

    vec.push_back(7);
    pushHeap(vec.begin(), vec.end());
    int ans2[10] = {9,7,8,3,5,0,2,3,1,4};
    for(int i = 0; i < vec.size(); i++) {
        assert((vec[i] == ans2[i]));
    }

    popHeap(vec.begin(), vec.end());
    assert((vec.back() == 9));
    vec.pop_back();
    int ans3[9] = {8,7,4,3,5,0,2,3,1};
    for(int i = 0; i < vec.size(); i++) {
        assert((vec[i] == ans3[i]));
    }

    sortHeap(vec.begin(), vec.end());
    int ans4[9] = {0,1,2,3,3,4,5,7,8};
    for(int i = 0; i < vec.size(); i++) {
        assert((vec[i] == ans4[i]));
    }

    int arr[9]={0,1,2,3,4,8,9,3,5};
    makeHeap(arr, arr+9);
    popHeap(arr, arr+9);
    assert((arr[8] == 9));

    // custom compare
    sortHeap(vec.begin(), vec.end(), greater<int>());
    int ans5[9] = {8,7,5,4,3,3,2,1,0};
    for(int i = 0; i < vec.size(); i++) {
        assert((vec[i] == ans5[i]));
    }
    cout << "end" << endl;
}