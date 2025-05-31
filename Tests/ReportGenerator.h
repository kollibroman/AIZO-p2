#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H
#include <memory>
#include <vector>
#include <bits/chrono.h>

#include "../Algorithms/BellmanFord.h"
#include "../Algorithms/Dijkstra.h"
#include "../Algorithms/FordFulkerson.h"
#include "../Algorithms/Kruskal.h"
#include "../Algorithms/Prim.h"
#include "../Infrastructure/GraphConverter.h"
#include "../Infrastructure/RandomGraphGenerator.h"
#include "../Utils/Data/GraphDataHandler.h"
#include "../Utils/Enums/GraphRepresentationType.h"

class ReportGenerator
{
public:
    ReportGenerator();

    static std::vector<GraphDataHandler> GenerateType1Report(const Algorithm algorithm, const int density, const int generationCount, const int vertexCount, const GraphRepresentationType representation)
    {
        auto type1Report = std::vector<GraphDataHandler>();

        for (int i = 0; i < generationCount; i++)
        {
            std::cout << "Generating graph " << i + 1 << " of " << generationCount << "\n";
            std::cout << "Algorithm: " << GetAlgorithmName(algorithm) << "\n";
            type1Report.push_back(*GenerateType1ReportEntry(algorithm, density, vertexCount, representation));
        }

        return type1Report;
    }

private:
    static std::string GetAlgorithmName(const Algorithm algorithm)
    {
        switch (algorithm)
        {
            case Algorithm::KRUSKAL: return "Kruskal's";
            case Algorithm::PRIM: return "Prim's";
            case Algorithm::DIJKSTRA: return "Dijkstra's";
            case Algorithm::BELLMAN_FORD: return "Bellman-Ford";
            case Algorithm::FORD_FULKERSON_DFS: return "Ford-Fulkerson (DFS)";
            case Algorithm::FORD_FULKERSON_BFS: return "Ford-Fulkerson (BFS)";
        }
    }

    static GraphDataHandler *GenerateType1ReportEntry(const Algorithm algorithm, const int density,
                                                           const int vertexCount,
                                                           GraphRepresentationType representation)
    {
        const auto graphGenerator = std::make_unique<RandomGraphGenerator>();

        switch (algorithm)
        {
            case Algorithm::KRUSKAL:
            {
                auto graph = GraphConverter::ConvertToRepresenstation(graphGenerator->generate(vertexCount, density, false), false, representation);

                const auto start_time = std::chrono::high_resolution_clock::now();
                KruskalAlgorithm<GraphRepresentation>::findMST(*graph);
                const auto end_time = std::chrono::high_resolution_clock::now();

                const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

                return new GraphDataHandler(algorithm, time, density, representation);
            }
            case Algorithm::PRIM:
            {
                auto graph = GraphConverter::ConvertToRepresenstation(graphGenerator->generate(vertexCount, density, false), false, representation);
                std::chrono::time_point<std::chrono::system_clock> start_time, end_time;

                if (representation == GraphRepresentationType::ADJACENCY_LIST)
                {
                     start_time = std::chrono::high_resolution_clock::now();
                    PrimList::findMST(*dynamic_cast<AdjacencyList*>(graph.get()));
                     end_time = std::chrono::high_resolution_clock::now();
                }
                else
                {
                     start_time = std::chrono::high_resolution_clock::now();
                    PrimMatrix::findMST(*dynamic_cast<AdjacencyMatrix*>(graph.get()));
                     end_time = std::chrono::high_resolution_clock::now();
                }

                 const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

                return new GraphDataHandler(algorithm, time, density, representation);
            }
            case Algorithm::DIJKSTRA:
            {
                auto graph = GraphConverter::ConvertToRepresenstation(graphGenerator->generate(vertexCount, density, true), true, representation);
                std::chrono::time_point<std::chrono::system_clock> start_time, end_time;

                if (representation == GraphRepresentationType::ADJACENCY_LIST)
                {
                    start_time = std::chrono::high_resolution_clock::now();
                    DijkstraList::findShortestPath(*dynamic_cast<AdjacencyList*>(graph.get()), 0);
                    end_time = std::chrono::high_resolution_clock::now();
                }
                else
                {
                    start_time = std::chrono::high_resolution_clock::now();
                    DijkstraMatrix::findShortestPath(*dynamic_cast<AdjacencyMatrix*>(graph.get()), 0);
                    end_time = std::chrono::high_resolution_clock::now();
                }

                const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

                return new GraphDataHandler(algorithm, time, density, representation);
            }
            case Algorithm::BELLMAN_FORD:
            {
                auto graph = GraphConverter::ConvertToRepresenstation(graphGenerator->generate(vertexCount, density, true), true, representation);
                std::chrono::time_point<std::chrono::system_clock> start_time, end_time;

                if (representation == GraphRepresentationType::ADJACENCY_LIST)
                {
                    start_time = std::chrono::high_resolution_clock::now();
                    BellmanFordList::findShortestPath(*dynamic_cast<AdjacencyList*>(graph.get()), 0);
                    end_time = std::chrono::high_resolution_clock::now();
                }
                else
                {
                    start_time = std::chrono::high_resolution_clock::now();
                    BellmanFordMatrix::findShortestPath(*dynamic_cast<AdjacencyMatrix*>(graph.get()), 0);
                    end_time = std::chrono::high_resolution_clock::now();
                }

                const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

                return new GraphDataHandler(algorithm, time, density, representation);
            }
            case Algorithm::FORD_FULKERSON_DFS:
            {
                auto graph = GraphConverter::ConvertToRepresenstation(graphGenerator->generate(vertexCount, density, true), true, representation);
                std::chrono::time_point<std::chrono::system_clock> start_time, end_time;

                if (representation == GraphRepresentationType::ADJACENCY_LIST)
                {
                    start_time = std::chrono::high_resolution_clock::now();
                    FordFulkersonList::findMaxFlow(*dynamic_cast<AdjacencyList*>(graph.get()), 0, vertexCount - 1, SearchMethod::DFS);
                    end_time = std::chrono::high_resolution_clock::now();
                }
                else
                {
                    start_time = std::chrono::high_resolution_clock::now();
                    FordFulkersonMatrix::findMaxFlow(*dynamic_cast<AdjacencyMatrix*>(graph.get()), 0, vertexCount - 1, SearchMethod::DFS);
                    end_time = std::chrono::high_resolution_clock::now();
                }

                const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

                return new GraphDataHandler(algorithm, time, density, representation);
            }
            case Algorithm::FORD_FULKERSON_BFS:
            {
                auto graph = GraphConverter::ConvertToRepresenstation(graphGenerator->generate(vertexCount, density, true), true, representation);
                std::chrono::time_point<std::chrono::system_clock> start_time, end_time;

                if (representation == GraphRepresentationType::ADJACENCY_LIST)
                {
                    start_time = std::chrono::high_resolution_clock::now();
                    FordFulkersonList::findMaxFlow(*dynamic_cast<AdjacencyList*>(graph.get()), 0, vertexCount - 1, SearchMethod::BFS);
                    end_time = std::chrono::high_resolution_clock::now();
                }
                else
                {
                    start_time = std::chrono::high_resolution_clock::now();
                    FordFulkersonMatrix::findMaxFlow(*dynamic_cast<AdjacencyMatrix*>(graph.get()), 0, vertexCount - 1, SearchMethod::BFS);
                    end_time = std::chrono::high_resolution_clock::now();
                }

                const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

                return new GraphDataHandler(algorithm, time, density, representation);
            }
        }
        return nullptr;
    }
};

#endif //REPORTGENERATOR_H
