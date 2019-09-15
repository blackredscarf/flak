#include <flak/Heap.h>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> vec{0,1,2,3,4,8,9,3,5};

    flak::makeHeap(vec.begin(), vec.end());
    for(int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
    // 9,5,8,3,4,0,2,3,1

    // push heap
    vec.push_back(7);
    flak::pushHeap(vec.begin(), vec.end());
    for(int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
    // 9,7,8,3,5,0,2,3,1,4

    // pop the top elements
    flak::popHeap(vec.begin(), vec.end());
    vec.pop_back();
    for(int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
    // 8,7,4,3,5,0,2,3,1

    // ascending sort
    flak::sortHeap(vec.begin(), vec.end());
    for(int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
    // 0,1,2,3,3,4,5,7,8

    // descending sort
    flak::sortHeap(vec.begin(), vec.end(), greater<int>());
    for(int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
    // 8,7,5,4,3,3,2,1,0
}
