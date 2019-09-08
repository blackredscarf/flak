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

#ifndef FLAK_DIJKSTRA_H
#define FLAK_DIJKSTRA_H

#include <cstddef>
#include <numeric>
#include <limits>
#include <queue>
#include <cstring>
#include "AdjacencyList.h"
using std::numeric_limits;
using std::priority_queue;

namespace flak {

template <typename EdgeValue>
class DijkstraEntry {
public:
    typedef size_t size_type;

    size_type vertex_;
    EdgeValue distance_;
    DijkstraEntry() = default;
    DijkstraEntry(size_type vertex_, EdgeValue distance_) : vertex_(vertex_), distance_(distance_) {}
    bool operator<(const DijkstraEntry<EdgeValue>& x) const {
        return distance_ < x.distance_;
    }
    bool operator>(const DijkstraEntry<EdgeValue>& x) const {
        return distance_ > x.distance_;
    }
};

template <typename GraphType, typename EdgeValue>
void dijkstra(GraphType& g, const size_t& source, EdgeValue* dis) {
    typedef DijkstraEntry<EdgeValue> Entry;
    const EdgeValue inf = numeric_limits<EdgeValue>::max();

    // min-heap
    priority_queue<Entry, vector<Entry>, std::greater<Entry>> qu;
    size_t num = g.vertexesSize();

    bool vis[num];
    std::fill(vis, vis + num, false);
    std::fill(dis, dis + num, inf);

    dis[source] = 0;
    qu.push(Entry(source, 0));

    while (!qu.empty()) {
        const Entry entry = qu.top();
        qu.pop();
        const size_t v = entry.vertex_;
        if(vis[v]) {
            continue;
        }
        vis[v] = true;
        if(dis[v] < entry.distance_) {
            continue;
        }
        typename GraphType::adjIterator it = g.adjacenciesBegin(v);
        for(; it != g.adjacenciesEnd(v); ++it) {
            const EdgeValue nowDis = dis[v] + g.edgeValue(it->edge());
            if(nowDis < dis[it->vertex()]) {
                dis[it->vertex()] = nowDis;
                qu.push(Entry(it->vertex(), nowDis));
            }
        }
    }

}

}




#endif //FLAK_DIJKSTRA_H
