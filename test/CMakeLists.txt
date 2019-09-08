# project name
set(PROJECT_NAME flak_test)

# version
set(FLAKTEST_VERSION_MAJOR 1)
set(FLAKTEST_VERSION_MINOR 0)
set(FLAKTEST_VERSION ${FLAKTEST_VERSION_MAJOR}.${FLAKTEST_VERSION_MINOR})

# cmake project
cmake_minimum_required(VERSION 3.13)
project(${PROJECT_NAME} VERSION ${FLAKTEST_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../include")

add_executable(TestAlg src/TestAlg.cpp)
add_executable(TestMergeSort src/TestMergeSort.cpp)
add_executable(TestSort src/TestSort.cpp)
add_executable(TestAVLTree src/TestAVLTree.cpp)
add_executable(TestAVLMap src/TestAVLMap.cpp)
add_executable(TestAVLSet src/TestAVLSet.cpp)
add_executable(TestHashTable src/TestHashTable.cpp)
add_executable(TestHeap src/TestHeap.cpp)
add_executable(TestList src/TestList.cpp)
add_executable(TestMap src/TestMap.cpp)
add_executable(TestPriorityQueue src/TestPriorityQueue.cpp)
add_executable(TestRBTree src/TestRBTree.cpp)
add_executable(TestSearchTree src/TestSearchTree.cpp)
add_executable(TestSet src/TestSet.cpp)
add_executable(TestSList src/TestSList.cpp)
add_executable(TestVector src/TestVector.cpp)
add_executable(TestHashSet src/TestHashSet.cpp)
add_executable(TestHashMap src/TestHashMap.cpp)
add_executable(TestKDTree src/TestKDTree.cpp)
add_executable(TestTrie src/TestTrie.cpp)

add_executable(TestAdjacenList src/graph/TestAdjacenList.cpp)
add_executable(TestDijstra src/graph/TestDijstra.cpp)
add_executable(TestBFS src/graph/TestBFS.cpp)
add_executable(TestDepthFirstSearch src/graph/TestDepthFirstSearch.cpp)
add_executable(TestDisjointSet src/graph/TestDisjointSet.cpp)
add_executable(TestRandomizedSelect src/TestRandomizedSelect.cpp)



