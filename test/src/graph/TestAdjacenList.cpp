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

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_set>
#include <flak/graph/AdjacencyList.h>
using namespace std;
using namespace flak;

void test1() {
    Graph<true> g;
    g.addVertex();
    g.addVertex();
    g.addVertex();
    g.addVertex();

    g.addEdge(0, 1); // 0
    g.addEdge(1, 2); // 1
    g.addEdge(1, 3); // 2
    g.addEdge(3, 2); // 3

    assert((g.edgesSize() == 4));
    assert((g.vertexesSize() == 4));

    auto it = g.adjacenciesBegin(1);
    int ans[2][2] = {{3, 2}, {2, 1}};
    int cnt = 0;
    for(; it != g.adjacenciesEnd(1); ++it) {
        assert((it->vertex() == ans[cnt][0]));
        assert((it->edge() == ans[cnt++][1]));
    }
    cout << endl;

    cout << "test 1 end" << endl;
}

void test2() {

    Graph<true, string, int> g;
    g.addVertex(22);
    g.addVertex(33);
    g.addVertex(44);
    g.addVertex(55);

    g.addEdge(0, 1, "edge0"); // 0
    g.addEdge(1, 2, "edge1"); // 1
    g.addEdge(1, 3, "edge2"); // 2
    g.addEdge(3, 2, "edge3"); // 3

    assert((g.edgesSize() == 4));
    assert((g.vertexesSize() == 4));

    auto it = g.adjacenciesBegin(1);
    int ans[2][2] = {{3, 2}, {2, 1}};
    string anse[2] = {"edge2", "edge1"};
    int ansv[2] = {55, 44};
    int cnt = 0;
    for(; it != g.adjacenciesEnd(1); ++it) {
        assert((it->vertex() == ans[cnt][0]));
        assert((g.vertexValue(it->vertex()) == ansv[cnt]));
        assert((it->edge() == ans[cnt][1]));
        assert((g.edgeValue(it->edge()) == anse[cnt]));
        cnt++;
    }
    cout << endl;

    cout << "test 2 end" << endl;
}

void test3() {
    Graph<true, string, int> g(20);
    assert((g.vertexesSize() == 20));
    cout << "test 1 end" << endl;
}

int main() {
//    test1();
//    test2();
    test3();
}

