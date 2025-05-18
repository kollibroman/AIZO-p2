#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include <opencv4/opencv2/core/types.hpp>
#include "../Utils/Results/PathResult.h"
#include "../Utils/BinaryHeap.h"
#include "../Utils/Representations/AdjacencyList.h"

constexpr int INF = std::numeric_limits<int>::max();

template<typename GraphType>
    static PathResult dijkstra(const GraphType& graph, int source)
    {
        int V = graph.getVertexCount();
        PathResult result;
        result.distances = DefinitelyNotAVector<int>(V, INF);
        result.predecessors = DefinitelyNotAVector<int>(V, -1);

        BinaryHeap<int> minHeap(V);
        minHeap.insert(0, source, source);
        result.distances[source] = 0;

        while (!minHeap.empty()) {
            auto min = minHeap.extractMin();
            int u = min.vertex;

            if constexpr (std::is_same_v<GraphType, AdjacencyList>) {
                for (const auto& edge : graph.getAdjacent(u)) {
                    int v = edge.destination;
                    int weight = edge.weight;
                    if (result.distances[u] != INF &&
                        result.distances[u] + weight < result.distances[v]) {
                        result.distances[v] = result.distances[u] + weight;
                        result.predecessors[v] = u;

                        if (minHeap.contains(v)) {
                            minHeap.decreaseKey(v, result.distances[v]);
                        } else {
                            minHeap.insert(result.distances[v], v, v);
                        }
                    }
                }
            } else {  // AdjacencyMatrix
                for (int v = 0; v < V; v++) {
                    int weight = graph.getWeight(u, v);
                    if (weight != graph.getNoEdgeValue() &&
                        result.distances[u] != INF &&
                        result.distances[u] + weight < result.distances[v]) {
                        result.distances[v] = result.distances[u] + weight;
                        result.predecessors[v] = u;

                        if (minHeap.contains(v)) {
                            minHeap.decreaseKey(v, result.distances[v]);
                        } else {
                            minHeap.insert(result.distances[v], v, v);
                        }
                    }
                }
            }
        }

    return result;
}

#endif //DIJKSTRA_H
