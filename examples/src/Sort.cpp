/*

Copyright 2019 flak authors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/


#include <flak/alg/Sort.h>
#include <flak/alg/InsertionSort.h>
#include <flak/alg/QuickSort.h>
#include <flak/alg/MergeSort.h>
#include <iostream>
using namespace std;

void mergeSort() {
    int arr[7] = {7, 4, 2, 5, 11, 8, 6};
    flak::mergeSort(arr, arr+7);
    for(int i = 0; i < 7; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    // 2 4 5 6 7 8 11
}

void insertionSort() {
    int arr[7] = {7, 4, 2, 5, 11, 8, 6};
    flak::insertionSort(arr, arr+7);
    for(int i = 0; i < 7; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    // 2 4 5 6 7 8 11

    flak::insertionSort(arr, arr+7, std::greater<int>());
    for(int i = 0; i < 7; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    // 11 8 7 6 5 4 2
}

void quickSort() {
    int arr[7] = {7, 4, 2, 5, 11, 8, 6};
    flak::quickSort(arr, arr+7);
    for(int i = 0; i < 7; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void Sort() {
    int arr[7] = {7, 4, 2, 5, 11, 8, 6};
    flak::Sort(arr, arr+7);
    for(int i = 0; i < 7; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    // 2 4 5 6 7 8 11
}

void partialSort() {
    int arr[7] = {7, 4, 2, 5, 11, 8, 6};
    // sort the first four elements.
    flak::partialSort(arr, arr+4, arr+7);
    for(int i = 0; i < 7; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    // 2 4 5 6 11 8 7
}

int main() {
    mergeSort();
    insertionSort();
    quickSort();
    Sort();
    partialSort();
}

