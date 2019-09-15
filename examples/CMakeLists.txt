# project name
set(PROJECT_NAME flak_examples)

# version
set(FLAKTEST_VERSION_MAJOR 1)
set(FLAKTEST_VERSION_MINOR 0)
set(FLAKTEST_VERSION ${FLAKTEST_VERSION_MAJOR}.${FLAKTEST_VERSION_MINOR})

# cmake project
cmake_minimum_required(VERSION 3.13)
project(${PROJECT_NAME} VERSION ${FLAKTEST_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../include")

add_executable(Map src/Map.cpp)
add_executable(Set src/Set.cpp)
add_executable(KDTree src/KDTree.cpp)
add_executable(Trie src/Trie.cpp)
add_executable(AdjacencyList src/AdjacencyList.cpp)
add_executable(DisjointSet src/DisjointSet.cpp)
add_executable(TestDijstra src/Dijstra.cpp)
add_executable(BFS src/BFS.cpp)
add_executable(DFS src/DFS.cpp)
add_executable(Sort src/Sort.cpp)
add_executable(Random src/Random.cpp)
add_executable(Heap src/Heap.cpp)
