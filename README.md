
<img src="https://i.loli.net/2019/09/08/Org1yiwa7SDAsGN.png" width="300px" /></a>

<hr>

**Flak** is a general and efficient data structures and algorithm template library. Implementation in STL style and easy-to-use interface.

## Container
A container is a holder object that stores a collection of other objects (its elements). They are implemented as class templates, which allows a great flexibility in the types supported as elements.

|             |         |           |         |               |            |
|-------------|---------|-----------|---------|---------------|------------|
| **Associative** | AVLTree | AVLMap    | AVLSet  | RBTree        | RBMap      |
|             | RBSet   | HashTable | HashMap | HashSet       | SearchTree |
|             | KDTree   | Trie |  |        |  |
|  **Sequential** | Vector  | List      | SList   | PriorityQueue |            |
|    **Other**    | Heap    |           |         |               |            |

## Algorithm
A collection of functions especially designed to be used on ranges of elements. A range is any sequence of objects that can be accessed through iterators or pointers, such as an array or an instance of some of the containers.

|        |               |               |           |           |   |
|--------|---------------|---------------|-----------|-----------|---|
| **Sort**   | MergeSort     | InsertionSort | QuickSort | IntroSort |   |
| **Common** | RandomShuffle | Reverse       | BinarySearch    | Merge     | Partition |
|            | RandomizedSelect |        |     |      |  |

## Graph
A library contained the graph data structures and algorithms. 

|        |               |               |           |           |   |
|--------|---------------|---------------|-----------|-----------|---|
| **Graph** | AdjacencyList |       |     |      |    |
| **Algorithm** |  Dijkstra       | BreadthFirstSearch    | DepthFirstSearch     |  DisjointSet  | |

## Install
git clone the repository.
```
mkdir build
cd build
cmake .. 
cmake --build ./ --target install --config=Release
```
In your `CMakeLists.txt`:
```cmake
find_package(flak 1.0.0 REQUIRED)

add_executable(YourLib YourPath/YourSource.cpp)
target_link_libraries(YourLib flak::flak)
```

## Sample
The AVLMap is implemented by AVLTree.
```cpp
#include<flak/AVLMap.h>
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

