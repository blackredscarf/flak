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

// KD Tree, you can learn it from https://en.wikipedia.org/wiki/K-d_tree
/*
 *  Every node is a point, and every node has a dim parameter to
 *  determine what dimension we will compare to for this node.
 *  If dim is 0, we compare the first dimension, go left if smaller, go right if greater.
 *  If dim is 1, we compare the second dimension, ...
 *  If dim reaches max dimension, come back to 0 and renew a round.
 *
 *
 * dim=0                40,50,100
 *                        / \
 *                       /   \
 * dim=1          (30),20,12  (70),36,7         (30 is smaller than 40, 70 is greater than 40)
 *                    / \
 *                   /   \
 * dim=2       15,(20),7  15,(45),7
 *                           / \
 *                          /   \
 * dim=0            15,20,(6)    13,21,(10)
 *                     / \
 *                    /   \
 * dim=1     (13),9,10   (19),40,10
 *
 * ...
 *
 */

#ifndef FLAK_KDTREE_H
#define FLAK_KDTREE_H

#include <iterator>
#include <stack>
#include <queue>
#include <utility>
#include <vector>
#include <limits>
#include "PriorityQueue.h"
using std::array;
using std::pair;
using std::stack;
using std::vector;
using std::make_pair;
using std::priority_queue;

namespace flak {

template<typename Point, typename Val, typename DimType>
class _KDQuery;

template<typename Val, typename DimType>
struct KDNode {
    typedef KDNode<Val, DimType> Self;
    typedef KDNode<Val, DimType>* SelfPtr;

    DimType dim_;
    SelfPtr low_;
    SelfPtr high_;
    Val value_;

    KDNode(const Val& val, DimType dim) :
            dim_(dim), low_(nullptr), high_(nullptr), value_(val) {}

    ~KDNode() {
        delete low_;
        delete high_;
    }

    bool operator<(const Self& x) const {
        return false;
    }
};

template<typename Val, typename DimType>
class KDNodeIterator {
public:
    typedef Val value_type;
    typedef Val* pointer;
    typedef Val& reference;
    typedef ptrdiff_t difference_type;

    typedef KDNodeIterator<Val, DimType> Self;
    typedef KDNode<Val, DimType>* NodePtr;

    NodePtr node_;
private:
    DimType dim_;

public:
    explicit KDNodeIterator(const NodePtr node) : node_(node) {}

    reference operator*() const {
        return node_->value_;
    }

    pointer operator->() const {
        return &(operator*());
    }

    bool operator<(const Self& x) const {
        return node_ < x.node_;
    }
};

template<typename Point, typename Val, typename DimType = size_t>
class KDTree {
public:
    typedef Point key_type;
    typedef Val mapped_type;
    typedef pair<const key_type, mapped_type> value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef size_t size_type;

    typedef DimType dim_type;
    typedef KDNode<value_type, DimType> Node;
    typedef KDNode<value_type, DimType>* NodePtr;
    typedef KDNodeIterator<value_type, DimType> iterator;
    typedef _KDQuery<Point, Val, DimType> Query;

private:
    NodePtr root_;
    size_type size_;
    DimType dimension_;
    static const key_type pointOf(NodePtr n) { return n->value_.first; };
    static mapped_type& valueOf(NodePtr n) { return n->value_.second; };

public:
    explicit KDTree(DimType dim): root_(nullptr), size_(0), dimension_(dim) {}
    size_type max_size() const { return std::numeric_limits<size_type>::max(); }
    size_type size() { return size_; }
    bool empty() { return size_ == 0; }
    ~KDTree() { delete root_; }
    void clear() {
        delete root_; // delete in recursion with ~KDNode()
        root_ = nullptr;
        size_ = 0;
    }

    // find the element with points of [p].
    pair<bool, iterator> find(Point& p) const {
        Query q(root_, p, dimension_, 1);
        typename Query::QueryValue qres = q.find(true);

        if(!qres.empty()) {
            typename Query::QDistanceValue v = qres.top();
            return make_pair(true, v.second);
        }
        return make_pair(false, iterator(nullptr));
    }

    // find the [k] nearest points of [p].
    // return a array with value of pair<size_type, iterator>.
    // the pair.first is the distance between two point.
    vector<pair<size_type, iterator>> findKNearest(Point& p, size_type k) const {
        Query q(root_, p, dimension_, k);
        typename Query::QueryValue qres = q.find(false);

        vector<pair<size_type, iterator>> res;
        res.reserve(qres.size());
        while(!qres.empty()) {
            res.push_back(qres.top());
            qres.pop();
        }
        return res;
    }

    // insert the [point] as key with [value]
    bool insert(const key_type& point, const mapped_type& value) {
        NodePtr parent;
        NodePtr node = _getNode(point, &parent);
        return _insert(point, value, &node, parent);
    }

    // erase the [point] and get the erased node value to [erasedValue]
    bool erase(key_type& point, mapped_type* const erasedValue = nullptr) {
        NodePtr parent;
        NodePtr node = _getNode(point, &parent);
        if(node == nullptr) {
            return false;
        }
        if(erasedValue != nullptr) {
            *erasedValue = valueOf(node);
        }
        _erase(node, parent);
        return true;
    }

private:

    // Get the position to insert, and get its [parent].
    // If have the same point, returning that point.
    NodePtr _getNode(const key_type& point, Node** parent) {
        dim_type dim;
        NodePtr node = root_;
        NodePtr prev = nullptr;

        while (node != nullptr) {
            if (pointOf(node) == point) { // same point
                if (parent != nullptr) {
                    *parent = prev;
                }
                return node;
            }

            dim = node->dim_;
            if (point[dim] > pointOf(node)[dim]) {
                prev = node;
                node = node->high_;
            } else if (point[dim] < pointOf(node)[dim]) {
                prev = node;
                node = node->low_;
            } else {            // equal situation
                prev = node;
                node = node->high_;
            }
        }

        if (parent != nullptr) {
            *parent = prev;
        }

        return nullptr;
    }

    // [point] is the point to insert
    // [node] is the position to insert
    bool _insert(const key_type& point, const mapped_type& value,
                 Node** node, Node* parent) {
        if (parent == nullptr) { // insert to root
            if (root_ != nullptr) {
                *node = root_;
            } else {
                root_ = *node = new Node(pair<key_type, mapped_type>(point, value), 0);
                ++size_;
                return false;
            }
        } else if (*node == nullptr) {
            DimType dim = parent->dim_;
            NodePtr& child = (point[dim] < pointOf(parent)[dim]) ? parent->low_ : parent->high_;
            if (++dim >= dimension_) {
                dim = 0;
            }
            child = *node = new Node(pair<key_type, mapped_type>(point, value), dim);
            ++size_;
            return false;
        }

        valueOf(*node) = value;
        return true;
    }

    void _erase(NodePtr node, NodePtr parent) {
        NodePtr susor = _findSuccessor(node);

        // susor link to erasing node children
        if(susor != nullptr) {
            susor->low_ = node->low_;
            susor->high_ = node->high_;
            susor->dim_ = node->dim_;
        }

        // erasing node parent link to susor
        if(parent == nullptr) {
            root_ = susor;
        } else if(node == parent->low_) {
            parent->low_ = susor;
        } else {
            parent->high_ = susor;
        }

        node->low_ = nullptr;
        node->high_ = nullptr;
        --size_;
        delete node;
    }

    // find the successor in recursion.
    // When you erase a node in a dim,
    // you need to find its one child as the successor.
    // But you also need to find a successor for the successor,
    // and in recursion until no sub-tree.
    NodePtr _findSuccessor(NodePtr node) {
        if(node == nullptr) {
            return nullptr;
        }

        DimType dim;
        NodePtr susor, parent;
        if(node->low_ == nullptr && node->high_ == nullptr) {
            return nullptr;
        } else {
            dim = node->dim_;
        }

        // If high is nullptr, let the low become high
        if(node->high_ == nullptr) {
            node->high_ = node->low_;
            node->low_ = nullptr;
        }

        susor = _getMinimumNode(node->high_, node, dim, &parent);

        // find a successor for the successor
        if(parent->low_ == susor) {
            parent->low_ = _findSuccessor(susor);
        } else {
            parent->high_ = _findSuccessor(susor);
        }
        return susor;
    }

    NodePtr _getMinimumNode(NodePtr node, NodePtr p, const DimType dim, Node** parent) {
        NodePtr res;
        if(dim == node->dim_) {
            if(node->low_ != nullptr) {
                return _getMinimumNode(node->low_, node, dim, parent);
            } else {
                res = node;
            }
        } else {
            NodePtr nlow = nullptr;
            NodePtr nhigh = nullptr;
            NodePtr plow = nullptr;
            NodePtr phigh = nullptr;
            if(node->low_ != nullptr) {
                nlow = _getMinimumNode(node->low_, node, dim, &plow);
            }
            if(node->high_ != nullptr) {
                nhigh = _getMinimumNode(node->high_, node, dim, &plow);
            }
            if(nlow != nullptr && nhigh != nullptr) {
                if(pointOf(nlow)[dim] < pointOf(nhigh)[dim]) {
                    res = nlow;
                    *parent = plow;
                } else {
                    res = nhigh;
                    *parent = phigh;
                }
            } else if (nlow != nullptr){
                res = nlow;
                *parent = plow;
            } else if (nhigh != nullptr) {
                res = nhigh;
                *parent = phigh;
            } else {
                res = node;
            }
        }

        if(res == node) {
            *parent = p;
        } else if(pointOf(node)[dim] < pointOf(res)[dim]) {
            res = node;
            *parent = p;
        }

        return res;
    }
};

template<typename Point, typename Val, typename DimType>
class _KDQuery {
public:
    typedef Point key_type;
    typedef Val mapped_type;
    typedef pair<const key_type, mapped_type> value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef size_t size_type;
    typedef DimType dim_type;
    typedef KDNode<value_type, DimType> Node;
    typedef KDNode<value_type, DimType>* NodePtr;
    typedef KDNodeIterator<value_type, DimType> iterator;

    typedef pair<size_type, iterator> QDistanceValue;

private:
    struct _less {
        bool operator()(const QDistanceValue& __x, const QDistanceValue& __y) const
        { return __x.first < __y.first; }
    };

    // use priority_queue to store the [k_] nearest  elements
    priority_queue<QDistanceValue, vector<QDistanceValue>, _less> Q;
    NodePtr root_;
    key_type query_;    // the key to query
    size_type k_;       // query the k nearest elements
    DimType dimension_; // the dimension of the key
    bool findItself_;

    // euclidean distance
    size_type _distance(const Point& p1, const Point& p2) {
        size_type sqsum = 0;
        for (int i = 0; i < dimension_; i++) {
            sqsum += (p2[i] - p1[i]) * (p2[i] - p1[i]);
        }
        return sqsum;
    }

    static const key_type pointOf(NodePtr n) { return n->value_.first; };

public:
    typedef priority_queue<QDistanceValue, vector<QDistanceValue>, _less> QueryValue;

    _KDQuery(NodePtr root, const key_type& query, const DimType maxDim, const size_type k)
            : root_(root), query_(query), dimension_(maxDim), k_(k), findItself_(false) {
    }

    QueryValue find(bool findItself) {
        findItself_ = findItself;
        findLoop(root_);
        return Q;
    }

private:
    void findLoop(NodePtr node) {
        if (node == nullptr)
            return;

        const DimType dim = node->dim_;

        // record whether find another side or not
        bool flag = false;

        NodePtr x = node->low_;
        NodePtr y = node->high_;

        size_type d = _distance(query_, pointOf(node));
        QDistanceValue value = pair<size_type, iterator>(d, iterator(node));
        if (query_[dim] >= pointOf(node)[dim]) {
            std::swap(x, y); // find the right side
        }

        if(x) {
            findLoop(x);
        }

        if (Q.size() < k_) {
            if(findItself_ && d == 0){
                Q.push(value);
                flag = true;
            } else if(!findItself_ && d != 0) {
                Q.push(value);
                flag = true;
            }
        } else {
            if(findItself_ && d == 0) {
                if (value.first < Q.top().first) {
                    Q.pop();
                    Q.push(value);
                }
            } else if(!findItself_ && d != 0) {
                if (value.first < Q.top().first) {
                    Q.pop();
                    Q.push(value);
                }
            }

            // If the query node is quite near to the bound of this dim,
            // we need to find in another side too.
            if ((query_[dim] - pointOf(node)[dim]) * (query_[dim] - pointOf(node)[dim])
                < Q.top().first) {
                flag = true;
            }
        }

        if (y && flag) { // find the another side
            findLoop(y);
        }
    }
};

}

#endif //FLAK_KDTREE_H
