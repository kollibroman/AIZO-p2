#ifndef MAXFLOWMENU_H
#define MAXFLOWMENU_H

#include "Abstractions/MenuBase.h"
#include "../Algorithms/FordFulkerson.h"
#include "../Utils/Results/FlowResult.h"

class MaxFlowMenu : public MenuBase {
public:
    MaxFlowMenu() : MenuBase() {}

    void show() {
        std::vector<std::string> entries = {
            "1. Load from file",
            "2. Generate random graph",
            "3. Display representations",
            "4. Run Ford-Fulkerson algorithm with BFS",
            "5. Run Ford-Fulkerson algorithm with DFS",
            "6. Back to main menu"
        };

        runMenu("Maximum Flow Problem", entries, [this](int selected) {
            switch (selected) {
                case 0:
                    loadFromFile(GraphProblemType::MAX_FLOW);
                    break;
                case 1: {
                    int vertices, density;
                    std::cout << "Enter number of vertices: ";
                    std::cin >> vertices;
                    std::cout << "Enter density (0-100): ";
                    std::cin >> density;
                    
                    // Clear input buffer
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
                    generateRandom(vertices, density, true);
                    break;
                }
                case 2:
                    displayBothRepresentations();
                    waitForEnter();
                    break;
                case 3:
                    if (graphLoaded) {
                        runFordFulkerson(SearchMethod::BFS);
                        waitForEnter();
                    } else {
                        std::cout << "No graph loaded!\n";
                        waitForEnter();
                    }
                    break;
                case 4:
                    if (graphLoaded) {
                        runFordFulkerson(SearchMethod::DFS);
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
    void runFordFulkerson(SearchMethod method) {
        int source, sink;
        std::cout << "Enter source vertex (0 to " << listGraph.getVertexCount() - 1 << "): ";
        std::cin >> source;
        std::cout << "Enter sink vertex (0 to " << listGraph.getVertexCount() - 1 << "): ";
        std::cin >> sink;
        
        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (source < 0 || source >= listGraph.getVertexCount() ||
            sink < 0 || sink >= listGraph.getVertexCount() ||
            source == sink) {
            std::cout << "Invalid vertices!\n";
            return;
        }

        std::string methodName = (method == SearchMethod::BFS) ? "BFS" : "DFS";
        std::cout << "\nRunning Ford-Fulkerson with " << methodName << " search method\n";

        auto listResult = FordFulkersonList::findMaxFlow(listGraph, source, sink, method);
        auto matrixResult = FordFulkersonMatrix::findMaxFlow(matrixGraph, source, sink, method);
        displayFlowResults("Ford-Fulkerson with " + methodName, listResult, matrixResult);
    }

    void displayFlowResults(const std::string& algorithm,
                          const FlowResult& listResult,
                          const FlowResult& matrixResult) {
        std::cout << "\n" << algorithm << " Algorithm Results:\n";

        std::cout << "\nList Representation Results:\n";
        displaySingleFlowResult(listResult);

        std::cout << "\nMatrix Representation Results:\n";
        displaySingleFlowResult(matrixResult);
    }

    void displaySingleFlowResult(const FlowResult& result) {
        std::cout << "Maximum Flow: " << result.maxFlow << "\n\n";
        std::cout << "Flow Graph:\n";
        for (int i = 0; i < result.flowGraph.size(); i++) {
            for (int j = 0; j < result.flowGraph[i].size(); j++) {
                if (result.flowGraph[i][j] > 0) {
                    std::cout << i << " -> " << j << ": "
                             << result.flowGraph[i][j] << "\n";
                }
            }
        }
        std::cout << "\n";
    }
};

#endif //MAXFLOWMENU_H