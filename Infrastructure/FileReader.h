#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <iostream>
#include "../Utils/GraphProblemType.h"
#include "../Utils/GraphData.h"
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h"

class FileReader {
public:
    static GraphData readGraphFromFile(const GraphProblemType& problemType) {
        std::string fileName;
        std::cout << "Enter the file name: ";
        std::cin >> fileName;

        std::ifstream file(fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + fileName);
        }

        GraphData graphData;

        // Read first line based on problem type
        switch (problemType) {
            case GraphProblemType::MST:
                file >> graphData.numEdges >> graphData.numVertices;
                graphData.startVertex = -1;
                graphData.endVertex = -1;
                break;

            case GraphProblemType::SHORTEST_PATH:
                file >> graphData.numEdges >> graphData.numVertices >> graphData.startVertex;
                graphData.endVertex = -1;
                break;

            case GraphProblemType::MAX_FLOW:
                file >> graphData.numEdges >> graphData.numVertices
                     >> graphData.startVertex >> graphData.endVertex;
                break;
        }

        // Read edges
        for (int i = 0; i < graphData.numEdges; i++) {
            int start, end, weight;
            file >> start >> end >> weight;

            graphData.edges.emplace_back(start, end, weight);

            // For MST, add reverse edge as it's undirected
            if (problemType == GraphProblemType::MST) {
                graphData.edges.emplace_back(end, start, weight);
            }
        }

        file.close();
        validateGraphData(graphData, problemType);
        return graphData;
    }

private:
    static void validateGraphData(const GraphData& data, const GraphProblemType& problemType) {
        if (data.numVertices < 0 || data.numEdges < 0) {
            throw std::invalid_argument("Number of vertices and edges must be non-negative");
        }

        for (const auto& edge : data.edges) {
            if (edge.first() < 0 || edge.first() >= data.numVertices ||
                edge.second() < 0 || edge.second() >= data.numVertices) {
                throw std::invalid_argument("Invalid vertex number detected");
            }
        }

        switch (problemType) {
            case GraphProblemType::SHORTEST_PATH:
                if (data.startVertex < 0 || data.startVertex >= data.numVertices) {
                    throw std::invalid_argument("Invalid start vertex for shortest path");
                }
                break;

            case GraphProblemType::MAX_FLOW:
                if (data.startVertex < 0 || data.startVertex >= data.numVertices ||
                    data.endVertex < 0 || data.endVertex >= data.numVertices ||
                    data.startVertex == data.endVertex) {
                    throw std::invalid_argument("Invalid start/end vertices for max flow");
                }
                break;

            default:
                break;
        }
    }
};


#endif //FILEREADER_H
