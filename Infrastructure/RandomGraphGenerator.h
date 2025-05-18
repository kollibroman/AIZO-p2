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
        // Validate inputs
        if (vertices <= 0) {
            throw std::invalid_argument("Number of vertices must be positive");
        }

        if (density < 0 || density > 100) {
            throw std::invalid_argument("Density must be between 0 and 100");
        }

        try {
            if (density >= 99) { // Handle high density cases with the complete graph generator
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
            int maxPossibleEdges;
            if (isDirected) {
                maxPossibleEdges = vertices * (vertices - 1);
            } else {
                maxPossibleEdges = (vertices * (vertices - 1)) / 2;
            }

            // Calculate how many edges we already have
            int existingEdges = isDirected ? graph.edges.size() : graph.edges.size() / 2;

            // Calculate how many more edges we need
            int targetEdges = (maxPossibleEdges * density) / 100;
            int remainingEdges = targetEdges - existingEdges;

            // Make sure we don't try to add a negative number of edges
            if (remainingEdges > 0) {
                addRandomEdges(graph, remainingEdges, isDirected, minWeight, maxWeight);
            }

            return graph;
        } catch (const std::exception& e) {
            std::cerr << "Error in generate: " << e.what() << std::endl;
            throw; // Re-throw to be handled by caller
        }
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
        // Validate input
        if (vertices <= 0) {
            throw std::invalid_argument("Number of vertices must be positive");
        }

        GraphData complete;
        complete.numVertices = vertices;

        try {
            // For undirected graphs, add edges only once (i,j) to prevent duplicates
            for (int i = 0; i < vertices; i++) {
                for (int j = 0; j < vertices; j++) {
                    if (i != j) { // Skip self-loops
                        int weight = std::uniform_int_distribution<>(minWeight, maxWeight)(gen);
                        if (isDirected || i < j) { // For undirected graphs, only add edges where i < j
                            complete.edges.emplace_back(i, j, weight);
                        }
                    }
                }
            }

            return complete;

        }
        catch (const std::exception& e) {
            std::cerr << "Error in generateComplete: " << e.what() << std::endl;
            throw; // Re-throw to be handled by caller
        }
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