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

#ifndef FLAK_ADJACENCYLIST_H
#define FLAK_ADJACENCYLIST_H

#include <iterator>
#include <vector>
#include <cassert>
#include <utility>
#include <set>
#include <list>
using std::list;
using std::vector;
using std::pair;
using std::set;
using std::make_pair;


namespace flak {

struct BlankVertexVal {

};

struct BlankEdgeVal {

};

class Edge {
public:
    size_t v1_;
    size_t v2_;

    Edge(size_t v1, size_t v2) : v1_(v1), v2_(v2) {}
};

class Adjacency {
public:
    typedef size_t size_type;
    size_type edge_;
    size_type vertex_;
public:
    Adjacency() = default;
    Adjacency(size_type vertex, size_type edge) : vertex_(vertex), edge_(edge) {}

    size_type edge() const {
        return edge_;
    }

    size_type vertex() const {
        return vertex_;
    }

    bool operator<(const Adjacency& adj) const {
        return vertex_ < adj.vertex();
    }
    bool operator==(const Adjacency& adj) const {
        return vertex_ == adj.vertex();
    }
};



template<bool isDirect, typename EdgeVal = BlankEdgeVal,
        typename VertexVal = BlankVertexVal>
class Graph {
public:
    typedef size_t size_type;
    typedef list<Adjacency> Adjacencies;
    typedef vector<Adjacencies>::iterator iterator;
    typedef Adjacencies::const_iterator adjIterator;

private:
    vector<Adjacencies> vertexes_;
    vector<Edge> edges_;
    vector<VertexVal> vertexVal_;
    vector<EdgeVal> edgeVal_;

public:
    Graph() = default;

    explicit Graph(size_type nodeNum) :
        vertexes_(nodeNum) {

    }

    size_type vertexesSize() const {
        return vertexes_.size();
    }

    size_type edgesSize() const {
        return edges_.size();
    }

    void resize(size_type nodeNum){
        vertexes_.resize(nodeNum);
    }

    iterator begin() {
        return vertexes_.begin();
    }

    adjIterator adjacenciesBegin(const size_type& idx) const  {
        return vertexes_[idx].begin();
    }

    iterator end() {
        return vertexes_.end();
    }

    adjIterator adjacenciesEnd(const size_type& idx) const {
        return vertexes_[idx].end();
    }

    // get value of a vertex
    VertexVal vertexValue(const size_type& idx) const {
        return vertexVal_[idx];
    }

    // get value of a edge
    EdgeVal edgeValue(const size_type& idx) const {
        return edgeVal_[idx];
    }

    void setVertexValue(const size_type& idx, const VertexVal& value) {
        if(idx < vertexesSize() && idx > vertexVal_.size()) {
            vertexVal_.resize(vertexesSize());
        }
        vertexVal_[idx] = value;
    }

    void setEdgeValue(const size_type& idx, const EdgeVal& value) {
        if(idx < edgesSize() && idx > edgeVal_.size()) {
            edgeVal_.resize(edgesSize());
        }
        edgeVal_[idx] = value;
    }

    size_type addVertex() {
        return addVertex(VertexVal());
    }

    size_type addVertex(VertexVal value) {
        vertexes_.push_back(Adjacencies());
        vertexVal_.push_back(value);
        return vertexes_.size() - 1;
    }

    size_type addEdge(size_type v1, size_type v2) {
        return addEdge(v1, v2, EdgeVal());
    }

    size_type addEdge(size_type v1, size_type v2, EdgeVal edgeVal) {
        assert(v1 < vertexesSize());
        assert(v2 < vertexesSize());
        edges_.push_back(Edge(v1, v2));
        edgeVal_.push_back(edgeVal);
        size_type edgeIndex = edges_.size() - 1;
        _insertEdgeToAdjacencies(edgeIndex);
        return edgeIndex;
    }

private:
    void _insertEdgeToAdjacencies(const size_type edgeIndex) {
        const Edge& edge = edges_[edgeIndex];
        size_type v1 = edge.v1_;
        size_type v2 = edge.v2_;
        if (isDirect) {
            vertexes_[v1].push_front(Adjacency(v2, edgeIndex));
        } else {
            vertexes_[v1].push_front(Adjacency(v2, edgeIndex));
            vertexes_[v2].push_front(Adjacency(v1, edgeIndex));
        }
    }

};

}

#endif //FLAK_ADJACENCYLIST_H
