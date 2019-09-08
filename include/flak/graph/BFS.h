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

#ifndef FLAK_BFS_H
#define FLAK_BFS_H

#include <queue>
#include <cassert>
#include <functional>
#include "AdjacencyList.h"
using std::queue;

namespace flak {

struct BFSContext {
    const size_t vertex;
    const int depth;
    bool interrupt;
    bool push;

    BFSContext(const size_t vertex, const int depth)
        : vertex(vertex), depth(depth), interrupt(false), push(true) {}
};

struct BFSEntry {
    size_t vertex;
    int depth;
    BFSEntry(size_t vertex, int depth) : vertex(vertex), depth(depth) {}
};

template<typename GraphType>
void breadthFirstSearch(const GraphType& g,
        const size_t start, const std::function<void(BFSContext&)>& callback) {

    assert((start < g.vertexesSize()));

    queue<BFSEntry> qu;

    BFSContext ctx (start, 0);
    callback(ctx);
    if(ctx.interrupt) {
        return;
    }
    if(ctx.push) {
        qu.push(BFSEntry(start, 0));
    }

    while (!qu.empty()) {
        const BFSEntry entry = qu.front();
        qu.pop();
        const size_t v = entry.vertex;
        const int depth = entry.depth + 1;

        typename GraphType::adjIterator it = g.adjacenciesBegin(v);
        for(; it != g.adjacenciesEnd(v); ++it) {
            BFSContext ctx (it->vertex(), depth);
            callback(ctx);
            if(ctx.interrupt) {
                return;
            }
            if(ctx.push) {
                qu.push(BFSEntry(it->vertex(), depth));
            }
        }
    }
}

}


#endif //FLAK_BFS_H
