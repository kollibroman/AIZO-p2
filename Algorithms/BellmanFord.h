#ifndef BELLMANFORD_H
#define BELLMANFORD_H
#include <cfloat>

#include "../Infrastructure/Edge.h"
#include "../Infrastructure/Node.h"
#include "../Utils/BigDVector.h"

inline bool findShortestPathBellmanFord(BigDVector<Node>& graph, int startNodeIndex)
{
    // Initialize distances
    for (int i = 0; i < graph.GetSize(); i++)
    {
        Node* node = graph.GetData(i);
        node->distance = DBL_MAX;
        node->predecessor = nullptr;
    }

    // Set distance of start node to 0
    graph.GetData(startNodeIndex)->distance = 0;

    // Create edge list
    BigDVector<Edge> edges;
    for (int i = 0; i < graph.GetSize(); i++)
    {
        Node* node = &graph[i];
        for (int j = 0; j < node->connections.GetSize(); j++)
        {
            Connection* conn = node->connections.GetData(j);
            edges.Push(Edge(node, conn->ptr, conn->distance));
        }
    }

    // Relax all edges |V| - 1 times
    for (int i = 0; i < graph.GetSize() - 1; i++)
    {
        for (int j = 0; j < edges.GetSize(); j++)
        {
            Edge* edge = &edges[j];
            Node* u = edge->source;
            Node* v = edge->destination;
            double weight = edge->weight;

            if (u->distance != DBL_MAX &&
                u->distance + weight < v->distance)
            {
                v->distance = u->distance + weight;
                v->predecessor = u;
            }
        }
    }

    // Check for negative weight cycles
    for (int i = 0; i < edges.GetSize(); i++)
    {
        Edge* edge = &edges[i];
        Node* u = edge->source;
        Node* v = edge->destination;
        double weight = edge->weight;

        if (u->distance != DBL_MAX &&
            u->distance + weight < v->distance)
        {
            return false; // Negative cycle detected
        }
    }

    return true; // No negative cycles
}

// Helper function to get the shortest path to a specific node
inline BigDVector<Node*> getPath(Node* destination)
{
    BigDVector<Node*> path;

    for (Node* current = destination; current != nullptr; current = current->predecessor)
    {
        path.Push(current);
    }

    return path;
}


#endif //BELLMANFORD_H
