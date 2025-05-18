#ifndef MSTMENU_H
#define MSTMENU_H

#include "Abstractions/MenuBase.h"
#include "../Algorithms/Prim.h"
#include "../Algorithms/Kruskal.h"

class MSTMenu : public MenuBase
{
public:
    MSTMenu() : MenuBase() {}

    void show() {
        std::vector<std::string> entries = {
            "1. Load from file",
            "2. Generate random graph",
            "3. Display representations",
            "4. Run Prim's algorithm",
            "5. Run Kruskal's algorithm",
            "6. Back to main menu"
        };

        runMenu("Minimum Spanning Tree Problem", entries, [this](int selected) {
            switch (selected) {
                case 0:
                    loadFromFile(GraphProblemType::MST);
                    break;
                case 1: {
                    int vertices, density;
                    std::cout << "Enter number of vertices: ";
                    std::cin >> vertices;
                    std::cout << "Enter density (0-100): ";
                    std::cin >> density;
                    
                    // Clear input buffer
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
                    generateRandom(vertices, density, false);
                    break;
                }
                case 2:
                    displayBothRepresentations();
                    waitForEnter();
                    break;
                case 3:
                    if (graphLoaded) {
                        runPrim();
                        waitForEnter();
                    } else {
                        std::cout << "No graph loaded!\n";
                        waitForEnter();
                    }
                    break;
                case 4:
                    if (graphLoaded) {
                        runKruskal();
                        waitForEnter();
                    } else {
                        std::cout << "No graph loaded!\n";
                        waitForEnter();
                    }
                    break;
                case 5:
                    return;
                default:
                    break;
            }
        });
    }

private:
    void runPrim() {
        auto listResult = PrimList::findMST(listGraph);
        auto matrixResult = PrimMatrix::findMST(matrixGraph);
        displayMSTResults("Prim's", listResult, matrixResult);
    }

    void runKruskal() {
        auto listResult = KruskalAlgorithm<AdjacencyList>::findMST(listGraph);
        auto matrixResult = KruskalAlgorithm<AdjacencyMatrix>::findMST(matrixGraph);
        displayMSTResults("Kruskal's", listResult, matrixResult);
    }

    void displayMSTResults(const std::string& algorithm,
                         const MSTResult& listResult,
                         const MSTResult& matrixResult) {
        std::cout << "\n" << algorithm << " Algorithm Results:\n";

        std::cout << "\nList Representation Results:\n";
        displaySingleMSTResult(listResult);

        std::cout << "\nMatrix Representation Results:\n";
        displaySingleMSTResult(matrixResult);
    }

    void displaySingleMSTResult(const MSTResult& result) {
        std::cout << "MST Edges:\n";
        for (const auto& edge : result.edges) {
            std::cout << edge.first() << " -- " << edge.second() << ""
                     << " (weight: " << edge.third() << ")\n";
        }
        std::cout << "Total MST Weight: " << result.totalWeight << "\n\n";
    }
};

#endif //MSTMENU_H