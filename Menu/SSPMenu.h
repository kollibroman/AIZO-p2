#ifndef SSPMENU_H
#define SSPMENU_H

#include "Abstractions/MenuBase.h"
#include "../Algorithms/Dijkstra.h"
#include "../Algorithms/BellmanFord.h"

class SSPMenu : public MenuBase {
public:
    SSPMenu() : MenuBase() {}

    void show() {
        std::vector<std::string> entries = {
            "1. Load from file",
            "2. Generate random graph",
            "3. Display representations",
            "4. Run Dijkstra's algorithm",
            "5. Run Bellman-Ford algorithm",
            "6. Back to main menu"
        };

        runMenu("Shortest Path Problem", entries, [this](int selected) {
            switch (selected) {
                case 0:
                    loadFromFile(GraphProblemType::SHORTEST_PATH);
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
                        runDijkstra();
                        waitForEnter();
                    } else {
                        std::cout << "No graph loaded!\n";
                        waitForEnter();
                    }
                    break;
                case 4:
                    if (graphLoaded) {
                        runBellmanFord();
                        waitForEnter();
                    } else {
                        std::cout << "No graph loaded!\n";
                        waitForEnter();
                    }
                    break;
                case 5:

                default:
                    break;
            }
        });
    }

private:
    void runDijkstra() {
        int source;
        std::cout << "Enter source vertex (0 to " << listGraph.getVertexCount() - 1 << "): ";
        std::cin >> source;
        
        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (source < 0 || source >= listGraph.getVertexCount()) {
            std::cout << "Invalid source vertex!\n";
            return;
        }

        auto listResult = DijkstraList::findShortestPath(listGraph, source);
        auto matrixResult = DijkstraMatrix::findShortestPath(matrixGraph, source);
        displayPathResults("Dijkstra's", listResult, matrixResult, source);
    }

    void runBellmanFord() {
        int source;
        std::cout << "Enter source vertex (0 to " << listGraph.getVertexCount() - 1 << "): ";
        std::cin >> source;
        
        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (source < 0 || source >= listGraph.getVertexCount()) {
            std::cout << "Invalid source vertex!\n";
            return;
        }

        auto listResult = BellmanFordList::findShortestPath(listGraph, source);
        auto matrixResult = BellmanFordMatrix::findShortestPath(matrixGraph, source);
        displayPathResults("Bellman-Ford", listResult, matrixResult, source);
    }

    void displayPathResults(const std::string& algorithm,
                          const PathResult& listResult,
                          const PathResult& matrixResult,
                          int source) {
        std::cout << "\n" << algorithm << " Algorithm Results:\n";

        std::cout << "\nList Representation Results:\n";
        displaySinglePathResult(listResult, source);

        std::cout << "\nMatrix Representation Results:\n";
        displaySinglePathResult(matrixResult, source);
    }

    void displaySinglePathResult(const PathResult& result, int source) {
        for (int i = 0; i < result.distances.size(); i++) {
            if (i != source) {
                std::cout << "Distance to vertex " << i << ": ";
                if (result.distances[i] == INF) {
                    std::cout << "∞";
                } else {
                    std::cout << result.distances[i];
                }
                std::cout << "\nPath: ";

                if (result.distances[i] == INF) {
                    std::cout << "No path exists";
                } else {
                    std::vector<int> path;
                    int current = i;
                    while (current != -1) {
                        path.push_back(current);
                        current = result.predecessors[current];
                    }
                    for (auto it = path.rbegin(); it != path.rend(); ++it) {
                        std::cout << *it;
                        if (std::next(it) != path.rend()) {
                            std::cout << " -> ";
                        }
                    }
                }
                std::cout << "\n\n";
            }
        }
    }
};

#endif //SSPMENU_H