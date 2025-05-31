#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include "../DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include "Edge.h"
#include "GraphRepresentation.h"

class AdjacencyList : public GraphRepresentation
{
private:
    DefinitelyNotAVector<DefinitelyNotAVector<Edge>> adjacencyList;
    int vertices;
    bool isDirected;

public:
    explicit AdjacencyList(int v, bool directed = false)
        : vertices(v), isDirected(directed) {
        adjacencyList = DefinitelyNotAVector<DefinitelyNotAVector<Edge>>(v);
        for (int i = 0; i < v; ++i) {
            adjacencyList[i] = DefinitelyNotAVector<Edge>();
        }
    }

    void addEdge(int from, int to, int weight) override {
        adjacencyList[from].push_back(Edge(from, to, weight));
        if (!isDirected) {
            adjacencyList[to].push_back(Edge(to, from, weight));
        }
    }

    const DefinitelyNotAVector<Edge>& getAdjacent(int vertex) const {
        return adjacencyList[vertex];
    }

    DefinitelyNotAVector<Edge> toEdgeList() const override {
        DefinitelyNotAVector<Edge> edges;
        for (int i = 0; i < vertices; ++i) {
            for (const auto& edge : adjacencyList[i]) {
                if (isDirected || edge.source <= edge.destination) {
                    edges.push_back(edge);
                }
            }
        }
        return edges;
    }

    int getVertexCount() const override { return vertices; }
    bool getIsDirected() const { return isDirected; }
};



#endif //ADJACENCYLIST_H
