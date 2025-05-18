#ifndef RANDOMGRAPHGENERATOR_H
#define RANDOMGRAPHGENERATOR_H
#include "../Utils/GraphData.h"
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include <random>
#include <algorithm>

class RandomGraphGenerator {
private:
    std::random_device rd;
    std::mt19937 gen;
    
public:
    RandomGraphGenerator() : gen(rd()) {}

    GraphData generate(const int vertices, const int density, const bool isDirected, const int minWeight = 1, const int maxWeight = 100) {
        if (density == 100) {
            return generateComplete(vertices, isDirected, minWeight, maxWeight);
        }
        
        GraphData graph;
        graph.numVertices = vertices;
        
        if (isDirected) {
            generateDirectedConnected(graph, minWeight, maxWeight);
        } else {
            generateUndirectedConnected(graph, minWeight, maxWeight);
        }

        // Calculate remaining edges to add based on density
        const int maxEdges = isDirected ? vertices * (vertices - 1) : (vertices * (vertices - 1)) / 2;
        const int targetEdges = (maxEdges * density) / 100;
        const int remainingEdges = targetEdges - graph.edges.size();

        // Add random edges to reach desired density
        addRandomEdges(graph, remainingEdges, isDirected, minWeight, maxWeight);

        return graph;
    }

private:
    void generateUndirectedConnected(GraphData& graph, int minWeight, int maxWeight) {
        // Generate spanning tree first
        DefinitelyNotAVector<int> vertices(graph.numVertices);
        for (int i = 0; i < graph.numVertices; i++) vertices[i] = i;
        std::ranges::shuffle(vertices, gen);

        // Add edges to form a spanning tree
        for (int i = 1; i < graph.numVertices; i++) {
            int from = vertices[i-1];
            int to = vertices[i];
            int weight = std::uniform_int_distribution<>(minWeight, maxWeight)(gen);
            graph.edges.emplace_back(from, to, weight);
        }
    }

    void generateDirectedConnected(GraphData& graph, int minWeight, int maxWeight) {
        // Generate a Hamiltonian cycle first
        DefinitelyNotAVector<int> vertices(graph.numVertices);
        for (int i = 0; i < graph.numVertices; i++) vertices[i] = i;
        std::ranges::shuffle(vertices, gen);

        // Create cycle
        for (int i = 0; i < graph.numVertices; i++) {
            int from = vertices[i];
            int to = vertices[(i + 1) % graph.numVertices];
            int weight = std::uniform_int_distribution<>(minWeight, maxWeight)(gen);
            graph.edges.emplace_back(from, to, weight);
        }
    }

    GraphData generateComplete(int vertices, bool isDirected, int minWeight, int maxWeight) {
        GraphData complete;
        complete.numVertices = vertices;

        for (int i = 0; i < vertices; i++) {
            for (int j = isDirected ? 0 : i + 1; j < vertices; j++) {
                if (i != j) {
                    int weight = std::uniform_int_distribution<>(minWeight, maxWeight)(gen);
                    complete.edges.emplace_back(i, j, weight);
                    if (!isDirected) {
                        complete.edges.emplace_back(j, i, weight);
                    }
                }
            }
        }

        // Remove 1% of edges for 99% density
        if (isDirected) {
            const int edgesToRemove = vertices * (vertices - 1) / 100;
            std::ranges::shuffle(complete.edges, gen);
            complete.edges.resize(complete.edges.size() - edgesToRemove);
        }

        return complete;
    }

    void addRandomEdges(GraphData& graph, int count, bool isDirected, int minWeight, int maxWeight) {
        std::uniform_int_distribution<> vertexDist(0, graph.numVertices - 1);
        std::uniform_int_distribution<> weightDist(minWeight, maxWeight);

        while (count > 0) {
            int from = vertexDist(gen);
            int to = vertexDist(gen);

            if (from != to && !edgeExists(graph, from, to)) {
                int weight = weightDist(gen);
                graph.edges.emplace_back(from, to, weight);
                if (!isDirected) {
                    graph.edges.emplace_back(to, from, weight);
                }
                count--;
            }
        }
    }

    static bool edgeExists(const GraphData& graph, int from, int to)
    {
        return std::ranges::any_of(graph.edges,
                                   [from, to](const DefinitelyNotATuple<int, int, int>& e)
                                   {
                                       return e.get<0>() == from && e.get<1>() == to;
                                   });
    }
};

#endif //RANDOMGRAPHGENERATOR_H