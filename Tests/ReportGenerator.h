#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H
#include <memory>
#include <vector>
#include <chrono>
#include <numeric>
#include <iostream>
#include <optional>

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
    ReportGenerator() = default;

    static std::vector<GraphDataHandler> GenerateType1Report(const Algorithm algorithm, const int density, const int generationCount, const int vertexCount, const GraphRepresentationType representation)
    {
        std::vector<long> type1ReportTimes; // Changed to long to avoid overflow
        type1ReportTimes.reserve(generationCount);

        for (int i = 0; i < generationCount; i++)
        {
            std::cout << "Generating graph " << i + 1 << " of " << generationCount << " ";
            std::cout << "Algorithm: " << GetAlgorithmName(algorithm) << " ";
            std::cout << "(" << GetRepresentationName(representation) << ")" << std::endl;
            
            auto result = GenerateType1ReportEntry(algorithm, density, vertexCount, representation);
            if (result.has_value()) {
                type1ReportTimes.push_back(result.value().MeasuredTime);
            } else {
                std::cerr << "Warning: Failed to generate report entry for " << GetAlgorithmName(algorithm) << std::endl;
                type1ReportTimes.push_back(0); // Add 0 as fallback
            }
        }

        // Calculate average time
        long averageTime = 0;
        if (!type1ReportTimes.empty()) {
            long totalTime = std::accumulate(type1ReportTimes.begin(), type1ReportTimes.end(), 0L);
            averageTime = totalTime / static_cast<long>(generationCount);
        }

        std::vector<GraphDataHandler> type1Report;
        type1Report.emplace_back(algorithm, averageTime, density, representation, vertexCount);

        std::cout << "Average time for " << GetAlgorithmName(algorithm) << " (" << GetRepresentationName(representation) 
                  << "): " << averageTime << " milisseconds" << std::endl;

        return type1Report;
    }

private:
    static std::string GetAlgorithmName(const Algorithm algorithm)
    {
        switch (algorithm)
        {
            case Algorithm::KRUSKAL: return "Kruskal";
            case Algorithm::PRIM: return "Prim";
            case Algorithm::DIJKSTRA: return "Dijkstra";
            case Algorithm::BELLMAN_FORD: return "Bellman-Ford";
            case Algorithm::FORD_FULKERSON_DFS: return "Ford-Fulkerson (DFS)";
            case Algorithm::FORD_FULKERSON_BFS: return "Ford-Fulkerson (BFS)";
            default: return "Unknown";
        }
    }
    
    static std::string GetRepresentationName(const GraphRepresentationType representation)
    {
        switch (representation)
        {
            case GraphRepresentationType::ADJACENCY_MATRIX: return "Matrix";
            case GraphRepresentationType::ADJACENCY_LIST: return "List";
            default: return "Unknown";
        }
    }

    static std::optional<GraphDataHandler> GenerateType1ReportEntry(const Algorithm algorithm, const int density,
                                                      const int vertexCount,
                                                      GraphRepresentationType representation)
    {
        try {
            const auto graphGenerator = std::make_unique<RandomGraphGenerator>();

            switch (algorithm)
            {
                case Algorithm::KRUSKAL:
                {
                    const auto graph = GraphConverter::ConvertToRepresenstation(graphGenerator->generate(vertexCount, density, false), false, representation);

                    const auto start_time = std::chrono::high_resolution_clock::now();
                    KruskalAlgorithm<GraphRepresentation>::findMST(*graph);
                    const auto end_time = std::chrono::high_resolution_clock::now();

                    const auto time = std::chrono::duration<double, std::milli>(end_time - start_time).count();

                    return GraphDataHandler(algorithm, time, density, representation, vertexCount);
                }
                case Algorithm::PRIM:
                {
                    const auto graph = GraphConverter::ConvertToRepresenstation(graphGenerator->generate(vertexCount, density, false), false, representation);
                    std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;

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

                     const auto time = std::chrono::duration<double, std::milli>(end_time - start_time).count();

                    return GraphDataHandler(algorithm, time, density, representation, vertexCount);
                }
                case Algorithm::DIJKSTRA:
                {
                    const auto graph = GraphConverter::ConvertToRepresenstation(graphGenerator->generate(vertexCount, density, true), true, representation);
                    std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;

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

                    const auto time = std::chrono::duration<double, std::milli>(end_time - start_time).count();

                    return GraphDataHandler(algorithm, time, density, representation, vertexCount);
                }
                case Algorithm::BELLMAN_FORD:
                {
                    const auto graph = GraphConverter::ConvertToRepresenstation(graphGenerator->generate(vertexCount, density, true), true, representation);
                    std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;

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

                    const auto time = std::chrono::duration<double, std::milli>(end_time - start_time).count();

                    return GraphDataHandler(algorithm, time, density, representation, vertexCount);
                }
                case Algorithm::FORD_FULKERSON_DFS:
                {
                    const auto graph = GraphConverter::ConvertToRepresenstation(graphGenerator->generate(vertexCount, density, true), true, representation);
                    std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;

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

                    const auto time = std::chrono::duration<double, std::milli>(end_time - start_time).count();

                    return GraphDataHandler(algorithm, time, density, representation, vertexCount);
                }
                case Algorithm::FORD_FULKERSON_BFS:
                {
                    const auto graph = GraphConverter::ConvertToRepresenstation(graphGenerator->generate(vertexCount, density, true), true, representation);
                    std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;

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

                    const auto time = std::chrono::duration<double, std::milli>(end_time - start_time).count();

                    return GraphDataHandler(algorithm, time, density, representation, vertexCount);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error in GenerateType1ReportEntry: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error in GenerateType1ReportEntry" << std::endl;
        }
        
        return std::nullopt;
    }
};

#endif //REPORTGENERATOR_H