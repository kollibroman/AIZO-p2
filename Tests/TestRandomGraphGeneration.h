#ifndef TESTRANDOMGRAPHGENERATION_H
#define TESTRANDOMGRAPHGENERATION_H
#include <iostream>
#include <memory>

class TestRandomGraphGeneration
{
    protected:
    AdjacencyList listGraph;
    AdjacencyMatrix matrixGraph;
    bool graphLoaded = false;

public:
    // Explicitly define a default constructor
    TestRandomGraphGeneration() : listGraph(0), matrixGraph(0), graphLoaded(false) {}

    // Virtual destructor for proper inheritance
    virtual ~TestRandomGraphGeneration() = default;

    void loadFromFile(const GraphProblemType type) {
        const auto graphData = FileReader::readGraphFromFile(type);
        convertToRepresentations(graphData, true);
        displayBothRepresentations();
    }

    void generateRandom(const int vertices, const int density, const bool isDirected) {
        const auto generator = std::make_unique<RandomGraphGenerator>();
        const auto graphData = generator->generate(vertices, density, isDirected);
        convertToRepresentations(graphData, isDirected);
        displayBothRepresentations();
    }

    void convertToRepresentations(const GraphData& data, const bool isDirected) {
        listGraph = *GraphConverter::graphDataToAdjList(data, isDirected);
        matrixGraph = *GraphConverter::graphDataToAdjMatrix(data, isDirected);
        graphLoaded = true;
    }


    void displayBothRepresentations() const {
        if (!graphLoaded) {
            std::cout << "No graph loaded!\n";
            return;
        }

        std::cout << "\nAdjacency List Representation:\n";
        displayListRepresentation();

        std::cout << "\nAdjacency Matrix Representation:\n";
        displayMatrixRepresentation();
    }

    void displayListRepresentation() const {
        // Implementation of list representation display
        for (int i = 0; i < listGraph.getVertexCount(); i++) {
            std::cout << i << " -> ";
            for (const auto& edge : listGraph.getAdjacent(i)) {
                std::cout << "(" << edge.destination << "," << edge.weight << ") ";
            }
            std::cout << "\n";
        }
    }

    void displayMatrixRepresentation() const {
        // Implementation of matrix representation display
        for (int i = 0; i < matrixGraph.getVertexCount(); i++) {
            for (int j = 0; j < matrixGraph.getVertexCount(); j++) {
                if (const int weight = matrixGraph.getWeight(i, j); weight == matrixGraph.getNoEdgeValue())
                {
                    std::cout << "∞ ";
                }
                else
                {
                    std::cout << weight << " ";
                }
            }
            std::cout << "\n";
        }
    }

    void clearScreen() const {
    #ifdef _WIN32
            system("cls");
    #else
            system("clear");
    #endif
    }
};

#endif //TESTRANDOMGRAPHGENERATION_H
