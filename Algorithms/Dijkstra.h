#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include "../Utils/Results/PathResult.h"
#include "../Utils/BinaryHeap.h"
#include "../Utils/Representations/AdjacencyList.h"
#include "../Utils/Representations/AdjancencyMatrix.h"

constexpr int INF = std::numeric_limits<int>::max();

class DijkstraList {
public:
    static PathResult findShortestPath(const AdjacencyList& graph, int source) {
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
        }
        return result;
    }
};

class DijkstraMatrix {
public:
    static PathResult findShortestPath(const AdjacencyMatrix& graph, int source) {
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
        return result;
    }
};

#endif //DIJKSTRA_H