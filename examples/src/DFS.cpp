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

#include <flak/graph/DFS.h>
#include <iostream>
#include <string>
using namespace std;

int main() {
    flak::Graph<true, flak::BlankEdgeVal, string> g(20);

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

    // start with vertex 1
    depthFirstSearch(
        g, 1,
        [&g](flak::DFSContext& ctx) {
            cout << ctx.vertex << " " << g.vertexValue(ctx.vertex) << " " << ctx.depth << endl;
        },
        nullptr
    );
    cout << endl << endl;

    // The third parameters a function that be called when the vertex is visited repeatedly.
    depthFirstSearch(
        g, 1,
        [&g](flak::DFSContext& ctx) {
            cout << ctx.vertex << " " << g.vertexValue(ctx.vertex) << " " << ctx.depth << endl;
        },
        [&g](flak::DFSContext& ctx) {
            cout << "repeated: " << ctx.vertex << " " << endl;
            ctx.visited = true; // you can control the visited status.
            ctx.interrupt = true;
        }
    );
}