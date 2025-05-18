#ifndef PRIM_H
#define PRIM_H

#include "../Utils/Node.h"
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include "../Utils/Results/MSTResult.h"
#include "../Utils/Representations/AdjacencyList.h"
#include "../Utils/BinaryHeap.h"

constexpr int INF = std::numeric_limits<int>::max();

 template<typename GraphType>
    static MSTResult prim(const GraphType& graph) {
        int V = graph.getVertexCount();
        MSTResult result;

        DefinitelyNotAVector<int> key(V, INF);
        DefinitelyNotAVector<int> parent(V, -1);
        DefinitelyNotAVector<bool> inMST(V, false);

        BinaryHeap<int> minHeap(V);
        minHeap.insert(0, 0, 0);
        key[0] = 0;

        while (!minHeap.empty()) {
            auto min = minHeap.extractMin();
            int u = min.vertex;
            inMST[u] = true;

            if (parent[u] != -1) {
                result.edges.emplace_back(parent[u], u, key[u]);
                result.totalWeight += key[u];
            }

            if constexpr (std::is_same_v<GraphType, AdjacencyList>) {
                for (const auto& edge : graph.getAdjacent(u)) {
                    int v = edge.destination;
                    int weight = edge.weight;

                    if (!inMST[v] && weight < key[v]) {
                        parent[v] = u;
                        key[v] = weight;

                        if (minHeap.contains(v)) {
                            minHeap.decreaseKey(v, weight);
                        } else {
                            minHeap.insert(weight, v, v);
                        }
                    }
                }
            } else {  // AdjacencyMatrix
                for (int v = 0; v < V; v++) {
                    int weight = graph.getWeight(u, v);
                    if (weight != graph.getNoEdgeValue() &&
                        !inMST[v] && weight < key[v]) {
                        parent[v] = u;
                        key[v] = weight;

                        if (minHeap.contains(v)) {
                            minHeap.decreaseKey(v, weight);
                        } else {
                            minHeap.insert(weight, v, v);
                        }
                    }
                }
            }
        }

        return result;
}


#endif //PRIM_H
