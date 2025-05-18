#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H
#include "../DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include "../Edge.h"

class AdjacencyList {
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

    void addEdge(int from, int to, int weight) {
        adjacencyList[from].push_back(Edge(from, to, weight));
        if (!isDirected) {
            adjacencyList[to].push_back(Edge(from, weight));
        }
    }

    const DefinitelyNotAVector<Edge>& getAdjacent(int vertex) const {
        return adjacencyList[vertex];
    }

    int getVertexCount() const { return vertices; }
    bool getIsDirected() const { return isDirected; }
};


#endif //ADJACENCYLIST_H
