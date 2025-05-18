#ifndef BELLMANFORD_H
#define BELLMANFORD_H
#include "../Utils/Results/PathResult.h"

class BellmanFordList {
public:
    static PathResult findShortestPath(const AdjacencyList& graph, int source) {
        int V = graph.getVertexCount();
        PathResult result;
        result.distances = DefinitelyNotAVector<int>(V, INF);
        result.predecessors = DefinitelyNotAVector<int>(V, -1);
        result.distances[source] = 0;

        for (int i = 0; i < V - 1; i++) {
            for (int u = 0; u < V; u++) {
                for (const auto& edge : graph.getAdjacent(u)) {
                    int v = edge.destination;
                    int weight = edge.weight;
                    if (result.distances[u] != INF && 
                        result.distances[u] + weight < result.distances[v]) {
                        result.distances[v] = result.distances[u] + weight;
                        result.predecessors[v] = u;
                    }
                }
            }
        }
        return result;
    }
};

class BellmanFordMatrix {
public:
    static PathResult findShortestPath(const AdjacencyMatrix& graph, int source) {
        int V = graph.getVertexCount();
        PathResult result;
        result.distances = DefinitelyNotAVector<int>(V, INF);
        result.predecessors = DefinitelyNotAVector<int>(V, -1);
        result.distances[source] = 0;

        for (int i = 0; i < V - 1; i++) {
            for (int u = 0; u < V; u++) {
                for (int v = 0; v < V; v++) {
                    int weight = graph.getWeight(u, v);
                    if (weight != graph.getNoEdgeValue() && 
                        result.distances[u] != INF && 
                        result.distances[u] + weight < result.distances[v]) {
                        result.distances[v] = result.distances[u] + weight;
                        result.predecessors[v] = u;
                    }
                }
            }
        }
        return result;
    }
};
#endif //BELLMANFORD_H