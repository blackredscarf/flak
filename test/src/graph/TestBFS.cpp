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

#include <flak/graph/BFS.h>
#include <cassert>
#include <iostream>
#include <string>
using namespace std;
using namespace flak;

void test1() {
    Graph<true, BlankEdgeVal, string> g(20);

    g.setVertexValue(1, "v1");
    g.setVertexValue(2, "v2");
    g.setVertexValue(3, "v3");
    g.setVertexValue(4, "v4");
    g.setVertexValue(5, "v5");
    g.setVertexValue(6, "v6");

    g.addEdge(1, 2);
    g.addEdge(1, 5);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(2, 3);
    g.addEdge(3, 5);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(4, 6);
    g.addEdge(5, 6);

    breadthFirstSearch(g, 1, [&g](BFSContext& ctx) {
        cout << ctx.vertex << " " << g.vertexValue(ctx.vertex) << " " << ctx.depth << endl;
    });
    cout << "test 1 end" << endl;
}

void test2() {
    Graph<false, BlankEdgeVal, string> g(20);

    g.setVertexValue(1, "v1");
    g.setVertexValue(2, "v2");
    g.setVertexValue(3, "v3");
    g.setVertexValue(4, "v4");
    g.setVertexValue(5, "v5");
    g.setVertexValue(6, "v6");

    g.addEdge(1, 2);
    g.addEdge(1, 5);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(2, 3);
    g.addEdge(3, 5);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(4, 6);
    g.addEdge(5, 6);

    breadthFirstSearch(g, 1, [&g](BFSContext& ctx) {
        cout << ctx.vertex << " " << g.vertexValue(ctx.vertex) << " " << ctx.depth << endl;
    });
    cout << "test 2 end" << endl;
}

int main() {
    test1();
    test2();
}

