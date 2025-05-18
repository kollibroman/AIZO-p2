#ifndef PRIM_H
#define PRIM_H

#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include "../Utils/Results/MSTResult.h"
#include "../Utils/BinaryHeap.h"

class PrimList {
public:
    static MSTResult findMST(const AdjacencyList& graph) {
        int V = graph.getVertexCount();
        MSTResult result;
        result.totalWeight = 0;

        DefinitelyNotAVector<bool> included(V, false);
        DefinitelyNotAVector<int> key(V, INT_MAX);
        DefinitelyNotAVector<int> parent(V, -1);
        
        BinaryHeap<int> minHeap(V);
        key[0] = 0;
        minHeap.insert(0, 0, 0);

        while (!minHeap.empty()) {
            int u = minHeap.extractMin().vertex;
            included[u] = true;

            for (const auto& edge : graph.getAdjacent(u)) {
                int v = edge.destination;
                int weight = edge.weight;

                if (!included[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                    
                    if (minHeap.contains(v)) {
                        minHeap.decreaseKey(v, weight);
                    } else {
                        minHeap.insert(weight, v, v);
                    }
                }
            }
        }

        // Construct result
        for (int i = 1; i < V; i++) {
            if (parent[i] != -1) {
                result.edges.push_back(DefinitelyNotATuple(parent[i], i, key[i]));
                result.totalWeight += key[i];
            }
        }

        return result;
    }
};

class PrimMatrix {
public:
    static MSTResult findMST(const AdjacencyMatrix& graph) {
        int V = graph.getVertexCount();
        MSTResult result;
        result.totalWeight = 0;

        DefinitelyNotAVector<bool> included(V, false);
        DefinitelyNotAVector<int> key(V, INT_MAX);
        DefinitelyNotAVector<int> parent(V, -1);
        
        BinaryHeap<int> minHeap(V);
        key[0] = 0;
        minHeap.insert(0, 0, 0);

        while (!minHeap.empty()) {
            int u = minHeap.extractMin().vertex;
            included[u] = true;

            for (int v = 0; v < V; v++) {
                int weight = graph.getWeight(u, v);
                
                if (weight != graph.getNoEdgeValue() && 
                    !included[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                    
                    if (minHeap.contains(v)) {
                        minHeap.decreaseKey(v, weight);
                    } else {
                        minHeap.insert(weight, v, v);
                    }
                }
            }
        }

        // Construct result
        for (int i = 1; i < V; i++) {
            if (parent[i] != -1) {
                result.edges.push_back(DefinitelyNotATuple<int, int, int>(parent[i], i, key[i]));
                result.totalWeight += key[i];
            }
        }

        return result;
    }
};
#endif //PRIM_H