
<img src="https://i.loli.net/2019/09/08/Org1yiwa7SDAsGN.png" width="300px" /></a>

<hr>

<p>
<a href="#"><img alt="GitHub" src="https://img.shields.io/github/license/blackredscarf/flak"></a>
<a href="#"><img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg" alt="PRs Welcome"></a>
</p>

**Flak** is a general and efficient data structures and algorithm template library. Implementation in STL style and easy-to-use interface.

## Container
A container is a holder object that stores a collection of other objects (its elements). They are implemented as class templates, which allows a great flexibility in the types supported as elements.

|             |         |           |         |               |            |
|-------------|---------|-----------|---------|---------------|------------|
| **Associative** | AVLTree [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/AVLTree.h) | AVLMap [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/AVLMap.h)    | AVLSet [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/AVLSet.h)  | RBTree [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/RBTree.h)  | RBMap [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/Map.h)  |
|             | RBSet [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/Set.h)  | HashTable [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/HashTable.h) | HashMap [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/HashMap.h) | HashSet [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/HashSet.h) | SearchTree [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/SearchTree.h) |
|             | KDTree [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/KDTree.h)   | Trie [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/Trie.h) |  |        |  |
|  **Sequential** | Vector [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/Vector.h) | List [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/List.h) | SList [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/SList.h) | PriorityQueue [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/PriorityQueue.h) |            |


## Algorithm
A collection of functions especially designed to be used on ranges of elements. A range is any sequence of objects that can be accessed through iterators or pointers, such as an array or an instance of some of the containers.

|        |               |               |           |           |   |
|--------|---------------|---------------|-----------|-----------|---|
| **Sort**   | MergeSort [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/alg/MergeSort.h) | InsertionSort [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/alg/InsertionSort.h) | QuickSort [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/alg/QuickSort.h) | IntroSort [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/alg/Sort.h) | partialSort [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/alg/Sort.h#L59)  |
| **Common** | RandomShuffle [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/alg/RandomShuffle.h) | Reverse [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/alg/Reverse.h)  | BinarySearch [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/alg/Search.h)  | Merge [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/alg/Merge.h)  | Partition [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/alg/Alg.h#L53) |
|            | RandomizedSelect [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/alg/RandomizedSelect.h) | Heap [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/Heap.h)  |     |      |  |

## Graph
A library contained the graph data structures and algorithms. 

|        |               |               |           |           |   |
|--------|---------------|---------------|-----------|-----------|---|
| **Graph** | AdjacencyList [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/graph/AdjacencyList.h) |       |     |      |    |
| **Algorithm** |  Dijkstra [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/graph/Dijkstra.h) | BreadthFirstSearch [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/graph/BFS.h) | DepthFirstSearch [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/graph/DFS.h) |  DisjointSet [(s)](https://github.com/blackredscarf/flak/blob/master/include/flak/graph/DisjointSet.h) | |

## Install
Git clone the repository.
```
git clone https://github.com/blackredscarf/flak
```
Using CMake to build.
```
cd flak
mkdir build
cd build
cmake .. 
cmake --build ./ --target install --config=Release
```
Import library to your project. In your `CMakeLists.txt`:
```cmake
find_package(flak 1.0.0 REQUIRED)

add_executable(YourLib YourPath/YourSource.cpp)
target_link_libraries(YourLib flak::flak)
```

## Sample
The AVLMap is implemented by AVLTree.
```cpp
#include <flak/AVLMap.h>
#include <iostream>
using namespace std;

int main() {
    // The interface is same as the STL map
    flak::AVLMap<string, int> smap;
    smap["jjhou"] = 1;
    smap["jerry"] = 2;
    smap["jason"] = 3;
    smap["jimmy"] = 4;

    flak::AVLMap<string, int>::iterator it1 = smap.begin();
    for(; it1 != smap.end(); it1++) {
        cout << it1->first << " : " << it1->second << endl;
    }

    smap.erase("jason");
    it1 = smap.find("jason");
    cout << (it1 == smap.end()) << endl;
}
```

