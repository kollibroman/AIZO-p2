#ifndef MENUBASE_H
#define MENUBASE_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <limits>
#include "../../Utils/Representations/AdjacencyList.h"
#include "../../Utils/Representations/AdjancencyMatrix.h"
#include "../../Infrastructure/FileReader.h"
#include "../../Infrastructure/RandomGraphGenerator.h"
#include "../../Infrastructure/GraphConverter.h"

class MenuBase
{
protected:
    AdjacencyList listGraph;
    AdjacencyMatrix matrixGraph;
    bool graphLoaded = false;

public:
    // Explicitly define a default constructor
    MenuBase() : listGraph(0), matrixGraph(0), graphLoaded(false) {}

    // Virtual destructor for proper inheritance
    virtual ~MenuBase() = default;

protected:

    void waitForEnter() const {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    // New simple menu implementation
    void runMenu(const std::string& title, const std::vector<std::string>& options, 
                 const std::function<void(int)>& handler) {
        int choice = 0;
        bool running = true;
        
        while (running) {
            
            // Display title
            std::cout << "\n=== " << title << " ===\n\n";
            
            // Display menu options
            for (size_t i = 0; i < options.size(); ++i) {
                std::cout << options[i] << "\n";
            }
            
            // Get user input
            std::cout << "\nEnter your choice: ";
            std::cin >> choice;
            
            // Clear input buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            // Validate input
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Press Enter to try again...";
                std::cin.get();
                continue;
            }
            
            // Adjust choice to 0-based index
            choice--;
            
            // Handle choice
            if (choice >= 0 && choice < static_cast<int>(options.size())) {
                handler(choice);
            } else {
                std::cout << "Invalid choice. Press Enter to try again...";
                std::cin.get();
            }
        }
    }

    void loadFromFile(const GraphProblemType type) {
        try {
            const auto graphData = FileReader::readGraphFromFile(type);
            convertToRepresentations(graphData, type == GraphProblemType::MAX_FLOW || type == GraphProblemType::SHORTEST_PATH);
            displayBothRepresentations();
            std::cout << "Graph loaded successfully!\n";
            waitForEnter();
        } catch (const std::exception& e) {
            std::cout << "Error loading graph: " << e.what() << "\n";
            waitForEnter();
        }
    }

    void generateRandom(const int vertices, const int density, const bool isDirected) {
        try {
            if (vertices <= 0) {
                std::cout << "Error: Number of vertices must be positive\n";
                waitForEnter();
                return;
            }
            
            if (density < 0 || density > 100) {
                std::cout << "Error: Density must be between 0 and 100\n";
                waitForEnter();
                return;
            }
            
            const auto generator = std::make_unique<RandomGraphGenerator>();
            const auto graphData = generator->generate(vertices, density, isDirected);
            convertToRepresentations(graphData, isDirected);
            displayBothRepresentations();
            std::cout << "Random graph generated successfully!\n";
            waitForEnter();
        } catch (const std::exception& e) {
            std::cout << "Error generating graph: " << e.what() << "\n";
            waitForEnter();
        }
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
        for (int i = 0; i < listGraph.getVertexCount(); i++) {
            std::cout << i << " -> ";
            for (const auto& edge : listGraph.getAdjacent(i)) {
                std::cout << "(" << edge.destination << "," << edge.weight << ") ";
            }
            std::cout << "\n";
        }
    }

    void displayMatrixRepresentation() const {
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
};

#endif //MENUBASE_H