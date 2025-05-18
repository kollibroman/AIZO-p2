#ifndef ADJANCENCYMATRIX_H
#define ADJANCENCYMATRIX_H
#include "../DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include "Edge.h"

class AdjacencyMatrix
{
private:
    DefinitelyNotAVector<DefinitelyNotAVector<int>> matrix;
    int vertices;
    static constexpr int NO_EDGE = -1;

public:
    explicit AdjacencyMatrix(int v) : vertices(v) {
        matrix = DefinitelyNotAVector<DefinitelyNotAVector<int>>(v);
        for (int i = 0; i < v; i++) {
            matrix[i] = DefinitelyNotAVector<int>(v, NO_EDGE);
        }
    }

    void addEdge(int from, int to, int weight) {
        matrix[from][to] = weight;
        matrix[to][from] = weight; // For undirected graph
    }

    int getWeight(int from, int to) const {
        return matrix[from][to];
    }

    int getVertexCount() const {
        return vertices;
    }

    int getNoEdgeValue() const {
        return NO_EDGE;
    }

    // Convert to edge list (used by Kruskal's algorithm)
    [[nodiscard]] DefinitelyNotAVector<Edge> toEdgeList() const {
        DefinitelyNotAVector<Edge> edges;
        for (int i = 0; i < vertices; i++) {
            for (int j = i + 1; j < vertices; j++) {
                if (matrix[i][j] != NO_EDGE) {
                    edges.push_back(Edge(i, j, matrix[i][j]));
                }
            }
        }
        return edges;
    }
};


#endif //ADJANCENCYMATRIX_H
