#ifndef GRAPHCONVERTER_H
#define GRAPHCONVERTER_H
#include <iomanip>
#include <iostream>
#include <memory>

class AdjacencyMatrix;
class AdjacencyList;

class GraphConverter {
public:
    static std::unique_ptr<AdjacencyList> graphDataToAdjList(const GraphData& data, bool isDirected) {
        auto adjList = std::make_unique<AdjacencyList>(data.numVertices, isDirected);

        for (const auto& edge : data.edges) {
            adjList->addEdge(edge.first(), edge.second(), edge.third());
        }

        return adjList;
    }

    static std::unique_ptr<AdjacencyMatrix> graphDataToAdjMatrix(const GraphData& data, bool isDirected) {
        auto adjMatrix = std::make_unique<AdjacencyMatrix>(data.numVertices, isDirected);

        for (const auto& edge : data.edges) {
            adjMatrix->addEdge(edge.first(), edge.second(), edge.third());
        }

        return adjMatrix;
    }

    static void printAdjacencyList(const AdjacencyList& graph) {
        std::cout << "Adjacency List Representation:\n";
        for (int i = 0; i < graph.getVertexCount(); ++i) {
            std::cout << "Vertex " << i << ": ";
            for (const auto& edge : graph.getAdjacent(i)) {
                std::cout << "(" << edge.destination << ", weight: " << edge.weight << ") ";
            }
            std::cout << "\n";
        }
    }

    static void printAdjacencyMatrix(const AdjacencyMatrix& graph) {
        std::cout << "Adjacency Matrix Representation:\n";
        std::cout << "   ";
        for (int i = 0; i < graph.getVertexCount(); ++i) {
            std::cout << std::setw(4) << i;
        }
        std::cout << "\n";

        for (int i = 0; i < graph.getVertexCount(); ++i) {
            std::cout << std::setw(2) << i << " ";
            for (int j = 0; j < graph.getVertexCount(); ++j) {
                int weight = graph.getWeight(i, j);
                if (weight == graph.getNoEdgeValue()) {
                    std::cout << std::setw(4) << "∞";
                } else {
                    std::cout << std::setw(4) << weight;
                }
            }
            std::cout << "\n";
        }
    }
};


#endif //GRAPHCONVERTER_H
