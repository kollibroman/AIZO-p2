#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include "../Utils/Results/PathResult.h"
#include "../Utils/Representations/AdjacencyList.h"
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h"

constexpr int INF = std::numeric_limits<int>::max();

template<typename GraphType>
static PathResult bellmanFord(const GraphType& graph, int source) {
        int V = graph.getVertexCount();
        PathResult result;
        result.distances = DefinitelyNotAVector<int>(V, INF);
        result.predecessors = DefinitelyNotAVector<int>(V, -1);
        result.hasNegativeCycle = false;

        result.distances[source] = 0;

        // Relax edges V-1 times
        for (int i = 0; i < V - 1; i++) {
            bool anyChange = false;

            if constexpr (std::is_same_v<GraphType, AdjacencyList>) {
                for (int u = 0; u < V; u++) {
                    for (const auto& edge : graph.getAdjacent(u)) {
                        if (result.distances[u] != INF &&
                            result.distances[u] + edge.weight < result.distances[edge.destination]) {
                            result.distances[edge.destination] = result.distances[u] + edge.weight;
                            result.predecessors[edge.destination] = u;
                            anyChange = true;
                        }
                    }
                }
            } else {  // AdjacencyMatrix
                for (int u = 0; u < V; u++) {
                    for (int v = 0; v < V; v++) {
                        int weight = graph.getWeight(u, v);
                        if (weight != graph.getNoEdgeValue() &&
                            result.distances[u] != INF &&
                            result.distances[u] + weight < result.distances[v]) {
                            result.distances[v] = result.distances[u] + weight;
                            result.predecessors[v] = u;
                            anyChange = true;
                        }
                    }
                }
            }

            if (!anyChange) break;
        }

        // Check for negative cycles
        if constexpr (std::is_same_v<GraphType, AdjacencyList>) {
            for (int u = 0; u < V; u++) {
                for (const auto& edge : graph.getAdjacent(u)) {
                    if (result.distances[u] != INF &&
                        result.distances[u] + edge.weight < result.distances[edge.destination]) {
                        result.hasNegativeCycle = true;
                        return result;
                    }
                }
            }
        } else {  // AdjacencyMatrix
            for (int u = 0; u < V; u++) {
                for (int v = 0; v < V; v++) {
                    int weight = graph.getWeight(u, v);
                    if (weight != graph.getNoEdgeValue() &&
                        result.distances[u] != INF &&
                        result.distances[u] + weight < result.distances[v]) {
                        result.hasNegativeCycle = true;
                        return result;
                    }
                }
            }
        }

        return result;
}



#endif //BELLMANFORD_H
