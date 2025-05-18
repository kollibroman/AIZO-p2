#ifndef REPRESENTATIONSTORER_H
#define REPRESENTATIONSTORER_H
#include <iostream>
#include <memory>

#include "Representations/AdjacencyList.h"
#include "Representations/AdjancencyMatrix.h"
#include "GraphData.h"
#include "GraphProblemType.h"
#include "../Infrastructure/GraphConverter.h"

class RepresentationStorer {
private:
    std::unique_ptr<AdjacencyList> adjList;
    std::unique_ptr<AdjacencyMatrix> adjMatrix;
    GraphProblemType problemType;

public:
    GraphStorage(const GraphData& data, GraphProblemType type) : problemType(type) {
        bool isDirected = (type != GraphProblemType::MST);
        adjList = GraphConverter::graphDataToAdjList(data, isDirected);
        adjMatrix = GraphConverter::graphDataToAdjMatrix(data, isDirected);
    }

    [[nodiscard]] const AdjacencyList& getAdjacencyList() const { return *adjList; }
    [[nodiscard]] const AdjacencyMatrix& getAdjacencyMatrix() const { return *adjMatrix; }

    void printRepresentations() const {
        std::cout << "\nProblem type: " << getProblemTypeName() << "\n\n";
        GraphConverter::printAdjacencyList(*adjList);
        std::cout << "\n";
        GraphConverter::printAdjacencyMatrix(*adjMatrix);
    }

private:
    [[nodiscard]] std::string getProblemTypeName() const {
        switch (problemType) {
            case GraphProblemType::MST: return "Minimum Spanning Tree";
            case GraphProblemType::SHORTEST_PATH: return "Shortest Path";
            case GraphProblemType::MAX_FLOW: return "Maximum Flow";
            default: return "Unknown";
        }
    }
};


#endif //REPRESENTATIONSTORER_H
