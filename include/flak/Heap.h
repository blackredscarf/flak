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

#ifndef ALG_HEAP_H
#define ALG_HEAP_H

#include <iterator>
using std::iterator_traits;

namespace flak {

struct _ValueLessCompare {
    template<typename T>
    bool operator()(T it1, T it2) const {
        return it1 < it2;
    }
};

// Here you need to do is make the holeIndex node goes up in the tree,
//  and make the node bigger than its two son.
// [holeIndex] is the index of new inserted node,
//  which actually is the last element of vector.
// [first] is the start of the vector, which actually is index 0 of vector.
// [topIndex] is the root node of tree, in fact that is index 0.
// typename [Distance] is the distance represent of pointer,
//  which was got by iterator_traits::difference_type that usually is ptr_diff.
template<class RandomAccessIterator, class Distance, class T, class Compare>
void _pushHeap(RandomAccessIterator first,
        Distance holeIndex, Distance topIndex, T value, Compare& comp) {
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;   // percolate up
    }
    *(first + holeIndex) = value;
}

template<class RandomAccessIterator>
void pushHeap(RandomAccessIterator first, RandomAccessIterator last) {

    typedef typename iterator_traits<RandomAccessIterator>::value_type
            _ValueType;
    typedef typename iterator_traits<RandomAccessIterator>::difference_type
            _DistanceType;

    _ValueLessCompare comp;
    _pushHeap(first, _DistanceType((last - first) - 1),
            _DistanceType(0), _ValueType(*(last - 1)), comp);
}

template<class RandomAccessIterator, class ValueCompare>
void pushHeap(RandomAccessIterator first, RandomAccessIterator last, ValueCompare& comp) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type
            _ValueType;
    typedef typename iterator_traits<RandomAccessIterator>::difference_type
            _DistanceType;

    _pushHeap(first, _DistanceType((last - first) - 1),
              _DistanceType(0), _ValueType(*(last - 1)), comp);
}

// This function is used to move the hole node to the right place.
// [holeIndex] is the position of node that need to percolate down.
// [len] is last - first, the range of heap.
// [value] is the value of hole node,
//  you need to help this value find a place.
//  The way is percolate it down to the lowest, and _pushHeap() it.
template<class RandomAccessIterator, class Distance, class T, class Compare>
void _adjustHeap(RandomAccessIterator first, Distance holeIndex,
        Distance len, T value, Compare comp) {
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;   // right son
    while (secondChild < len) {
        // compare the left and right son
        if(comp(*(first + secondChild), *(first + (secondChild - 1)))) {
            secondChild--;
        }
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        // continue go down (percolate down)
        secondChild = 2 * (secondChild + 1);
    }
    if(secondChild == len) {    // the last node, and which has no right son
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    // Above, we make the value move to the lowest position.
    // And then, we need to percolate it up to the appropriate position (smaller than parent and bigger than two son).
    _pushHeap(first, holeIndex, topIndex, value, comp);
}

// If we pop the root node, we put it to the last.
// And the root node will become a hole node with [value], we will adjust it to the right position.
template <class RandomAccessIterator>
void popHeap(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type _ValueType;
    typedef typename iterator_traits<RandomAccessIterator>::difference_type _DistanceType;

    RandomAccessIterator result = last - 1; // the position of last element
    _ValueType value = *(last - 1); // the value of last element
    *result = *first;   // make the root node to the last node.
    RandomAccessIterator trueLast = last - 1;

    _ValueLessCompare comp;
    _adjustHeap(first, _DistanceType(0),
            _DistanceType(trueLast - first), value, comp);
}


// copy the [first] element to [result], and help the value find a place in the heap.
template <class RandomAccessIterator, class T, class ValueCompare>
void popHeap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, ValueCompare comp) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type _ValueType;
    typedef typename iterator_traits<RandomAccessIterator>::difference_type _DistanceType;

    *result = *first;
    _adjustHeap(first, _DistanceType(0),
                _DistanceType(last - first), value, comp);
}

template <class RandomAccessIterator, class ValueCompare>
void popHeap(RandomAccessIterator first, RandomAccessIterator last, ValueCompare comp) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type _ValueType;
    typedef typename iterator_traits<RandomAccessIterator>::difference_type _DistanceType;

    RandomAccessIterator result = last - 1; // the position of last element
    _ValueType value = *(last - 1); // the value of last element
    *result = *first;   // make the root node to the last node.
    RandomAccessIterator trueLast = last - 1;

    _adjustHeap(first, _DistanceType(0),
                _DistanceType(trueLast - first), value, comp);
}

// When we constantly pop, the tree(vector) will be sorted as a increasing sequence.
// Because the max value will continuously move to the last.
template <class RandomAccessIterator>
void sortHeap(RandomAccessIterator first, RandomAccessIterator last) {
    while(last - first > 1) {
        popHeap(first, last--);
    }
}
template <class RandomAccessIterator, class ValueCompare>
void sortHeap(RandomAccessIterator first, RandomAccessIterator last, ValueCompare comp) {
    while(last - first > 1) {
        popHeap(first, last--, comp);
    }
}


// make a container become a heap.
template <class RandomAccessIterator>
void makeHeap(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type _ValueType;
    typedef typename iterator_traits<RandomAccessIterator>::difference_type _DistanceType;
    _ValueLessCompare comp;

    if(last - first < 2){ // unnecessary for a sequence whose len is 0 or 1.
        return;
    }
    _DistanceType len = last - first;

    // len - 1 is the position of last element.
    // (node - 1) / 2 is parent of node.
    // Here parent is the parent of last node.
    _DistanceType parent = (len - 2) / 2;

    while (true) {
        // adjust the sub-tree with parent as the root node.
        _adjustHeap(first, parent, len, _ValueType(*(first + parent)), comp);
        if(parent == 0){  // after the root node of whole tree adjusted, over.
            return;
        }
        parent--;   // find the children node.
    }
}
template <class RandomAccessIterator, class ValueCompare>
void makeHeap(RandomAccessIterator first, RandomAccessIterator last, ValueCompare comp) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type _ValueType;
    typedef typename iterator_traits<RandomAccessIterator>::difference_type _DistanceType;
    if(last - first < 2){ // unnecessary for a sequence whose len is 0 or 1.
        return;
    }
    _DistanceType len = last - first;
    _DistanceType parent = (len - 2) / 2;
    while (true) {
        _adjustHeap(first, parent, len, _ValueType(*(first + parent)), comp);
        if(parent == 0){
            return;
        }
        parent--;
    }
}

}

#endif //ALG_HEAP_H
