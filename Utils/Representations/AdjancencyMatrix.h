#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H
#include "../DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include "Edge.h"
#include "GraphRepresentation.h"

class AdjacencyMatrix : public GraphRepresentation
{
private:
    DefinitelyNotAVector<DefinitelyNotAVector<int>> matrix;
    int vertices;
    bool isDirected;
    static constexpr int NO_EDGE = -1;

public:
    explicit AdjacencyMatrix(const int v, bool directed = false) : vertices(v), isDirected(directed) {
        matrix = DefinitelyNotAVector<DefinitelyNotAVector<int>>(v);
        for (int i = 0; i < v; i++) {
            matrix[i] = DefinitelyNotAVector<int>(v, NO_EDGE);
        }
    }

    void addEdge(int from, int to, int weight) override {
        matrix[from][to] = weight;
        if (!isDirected) {
            matrix[to][from] = weight; // For undirected graph only
        }
    }

    [[nodiscard]] int getWeight(int from, int to) const {
        return matrix[from][to];
    }

    [[nodiscard]] int getVertexCount() const override {
        return vertices;
    }

    [[nodiscard]] static int getNoEdgeValue()
    {
        return NO_EDGE;
    }

    // Convert to edge list (used by Kruskal's algorithm)
    [[nodiscard]] DefinitelyNotAVector<Edge> toEdgeList() const override {
        DefinitelyNotAVector<Edge> edges;
        for (int i = 0; i < vertices; i++) {
            for (int j = isDirected ? 0 : i + 1; j < vertices; j++) {
                if (i != j && matrix[i][j] != NO_EDGE) {
                    edges.push_back(Edge(i, j, matrix[i][j]));
                }
            }
        }
        return edges;
    }
};

#endif //ADJACENCYMATRIX_H