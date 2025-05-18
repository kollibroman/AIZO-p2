#ifndef FORDFULKERSON_H
#define FORDFULKERSON_H

#include "../Utils/Node.h"
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAQueue.h"
#include "../Utils/Representations/AdjacencyList.h"

template<typename GraphType>
    static FlowResult fordFulkerson(const GraphType& graph, int source, int sink) {
        int V = graph.getVertexCount();
        FlowResult result;

        // Create residual graph
        DefinitelyNotAVector<DefinitelyNotAVector<int>> residual(V, DefinitelyNotAVector<int>(V, 0));

        // Initialize residual graph
        if constexpr (std::is_same_v<GraphType, AdjacencyList>) {
            for (int u = 0; u < V; u++) {
                for (const auto& edge : graph.getAdjacent(u)) {
                    residual[u][edge.destination] = edge.weight;
                }
            }
        } else {  // AdjacencyMatrix
            for (int u = 0; u < V; u++) {
                for (int v = 0; v < V; v++) {
                    int weight = graph.getWeight(u, v);
                    if (weight != graph.getNoEdgeValue()) {
                        residual[u][v] = weight;
                    }
                }
            }
        }

        DefinitelyNotAVector<int> parent(V);

        // Augment flow while there is a path
        while (bfs(residual, source, sink, parent)) {
            int pathFlow = INF;

            // Find minimum residual capacity along the path
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = std::min(pathFlow, residual[u][v]);
            }

            // Update residual capacities
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                residual[u][v] -= pathFlow;
                residual[v][u] += pathFlow;
            }

            result.maxFlow += pathFlow;
        }

        // Store final flow values
        if constexpr (std::is_same_v<GraphType, AdjacencyList>) {
            for (int u = 0; u < V; u++) {
                for (const auto& edge : graph.getAdjacent(u)) {
                    int v = edge.destination;
                    int capacity = edge.weight;
                    int flow = capacity - residual[u][v];
                    if (flow > 0) {
                        result.flowEdges.emplace_back(u, v, flow);
                    }
                }
            }
        } else {  // AdjacencyMatrix
            for (int u = 0; u < V; u++) {
                for (int v = 0; v < V; v++) {
                    int capacity = graph.getWeight(u, v);
                    if (capacity != graph.getNoEdgeValue()) {
                        int flow = capacity - residual[u][v];
                        if (flow > 0) {
                            result.flowEdges.emplace_back(u, v, flow);
                        }
                    }
                }
            }
        }

        return result;
    }

static bool bfs(const DefinitelyNotAVector<DefinitelyNotAVector<int>>& residual,
                   int source, int sink, DefinitelyNotAVector<int>& parent) {
    int V = residual.size();
    DefinitelyNotAVector<bool> visited(V, false);

    DefinitelyNotAQueue<int> queue;
    queue.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();

        for (int v = 0; v < V; v++) {
            if (!visited[v] && residual[u][v] > 0) {
                queue.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    return visited[sink];
}


#endif //FORDFULKERSON_H
