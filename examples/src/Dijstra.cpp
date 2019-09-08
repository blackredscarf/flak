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

#include <flak/graph/Dijkstra.h>
#include <iostream>
using namespace std;

int main() {
    int vs = 20, trg = 5;
    flak::Graph<true, int> g(vs);
    g.addEdge(1, 2, 2);
    g.addEdge(1, 5, 3);
    g.addEdge(1, 3, 4);
    g.addEdge(2, 4, 7);
    g.addEdge(2, 3, 5);
    g.addEdge(3, 5, 1);
    g.addEdge(3, 4, 2);
    g.addEdge(4, 5, 1);
    g.addEdge(4, 6, 3);
    g.addEdge(5, 6, 3);

    int dis[vs];

    dijkstra(g, 2, dis);
    cout << dis[trg] << endl; // 6

    dijkstra(g, 1, dis);
    cout << dis[trg] << endl; // 3

    dijkstra(g, 1, dis);
    cout << dis[6] << endl; // 6
}
