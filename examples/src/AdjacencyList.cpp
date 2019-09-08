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

#include <iostream>
#include <unordered_set>
#include <flak/graph/AdjacencyList.h>
using namespace std;

void insertEdge() {
    // Building a directed graph when first templated parameter is true,
    // otherwise a undirected graph.
    flak::Graph<true> g;
    g.addVertex();
    g.addVertex();
    g.addVertex();
    g.addVertex();

    g.addEdge(0, 1); // 0
    g.addEdge(1, 2); // 1
    g.addEdge(1, 3); // 2
    g.addEdge(3, 2); // 3

    // the edge of vertex 1
    auto it = g.adjacenciesBegin(1);
    for(; it != g.adjacenciesEnd(1); ++it) {
        cout << it->vertex() << " ";
        cout << it->edge() << endl;
    }
    cout << endl;
}

void edgeValue() {
    // The second parameter is the value type of edge.
    // The third parameter is the value type of vertex.
    flak::Graph<true, string, int> g;
    g.addVertex(22); // the value of vertex 0 is 22
    g.addVertex(33);
    g.addVertex(44);
    g.addVertex(55);

    g.addEdge(0, 1, "edge0"); // 0
    g.addEdge(1, 2, "edge1"); // 1
    g.addEdge(1, 3, "edge2"); // 2
    g.addEdge(3, 2, "edge3"); // 3

    auto it = g.adjacenciesBegin(1);
    for(; it != g.adjacenciesEnd(1); ++it) {
        cout << it->vertex() << " ";
        cout << it->edge() << " ";
        cout << g.edgeValue(it->edge()) << endl; // edge value
    }
    cout << endl;
}

void specifySize() {
    flak::Graph<true, string, int> g(20);
    cout << g.vertexesSize() << endl; // 20
    cout << g.edgesSize() << endl; // 0
}


int main() {
    insertEdge();
    edgeValue();
    specifySize();
}


