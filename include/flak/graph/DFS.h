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

#ifndef FLAK_DFS_H
#define FLAK_DFS_H

#include <stack>
#include <functional>
#include "AdjacencyList.h"


namespace flak {

struct DFSContext {
    const size_t vertex;
    const int depth;
    bool visited;
    bool interrupt;
    bool pushNeighbors;

    DFSContext(const size_t vertex, const int depth)
            : vertex(vertex), depth(depth), visited(true), interrupt(false), pushNeighbors(true) {}
};

struct DFSEntry {
    size_t vertex;
    int depth;
    DFSEntry(size_t vertex, int depth) : vertex(vertex), depth(depth) {}
};

template<typename GraphType>
void depthFirstSearch(const GraphType& g,
        const size_t start,
        const std::function<void(DFSContext&)>& callback,
        const std::function<void(DFSContext&)>& repeatedVisitedCallback) {

    assert((start < g.vertexesSize()));
    bool vis[g.vertexesSize()];
    std::stack<DFSEntry> s;
    s.push(DFSEntry(start, 0));

    while (!s.empty()) {
        const DFSEntry entry = s.top();
        s.pop();

        const size_t v = entry.vertex;
        vis[v] = true;

        DFSContext ctx (v, entry.depth);
        callback(ctx);
        if(ctx.interrupt) {
            return;
        }
        if(ctx.pushNeighbors) {
            const int depth = entry.depth + 1;
            auto it = g.adjacenciesBegin(v);
            for(; it != g.adjacenciesEnd(v); ++it) {
                if(!vis[it->vertex()]) {
                    s.push(DFSEntry(it->vertex(), depth));
                } else {
                    if(repeatedVisitedCallback != nullptr) {
                        DFSContext rectx (it->vertex(), depth);
                        repeatedVisitedCallback(rectx);
                        if(rectx.interrupt) {
                            return;
                        } else if(rectx.visited) {
                            continue;
                        } else {
                            vis[it->vertex()] = false;
                        }
                    }
                }
            }
        }
    }
}


}

#endif //FLAK_DFS_H
