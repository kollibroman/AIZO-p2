#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include "../Utils/DefinitelyNotADataStructures/UnionFind.h"
#include "../Utils/Results/MSTResult.h"
#include "../Utils/Representations/Edge.h"
#include "Sorting.h"

template<typename GraphRepresentation>
class KruskalAlgorithm {
public:
    static MSTResult findMST(const GraphRepresentation& graph) {
        MSTResult result;

        // Convert graph representation to edge list
        auto edges = graph.toEdgeList();

        // Sort edges by weight
        Sorting::QuickSort(edges, 0, edges.size() - 1);

        // Initialize Union-Find data structure
        UnionFind uf(graph.getVertexCount());

        // Process edges in sorted order
        for (const Edge& edge : edges) {
            if (uf.find(edge.source) != uf.find(edge.destination)) {
                uf.unite(edge.source, edge.destination);
                result.edges.emplace_back(edge.source, edge.destination, edge.weight);
                result.totalWeight += edge.weight;
            }
        }

        return result;
    }
};


#endif //KRUSKAL_H
