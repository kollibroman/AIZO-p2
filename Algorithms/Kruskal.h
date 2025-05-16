#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "../Infrastructure/Edge.h"
#include "../Infrastructure/Node.h"
#include "../Utils/BigDVector.h"

struct Edge;
// Find set of vertex i
inline Node* findSet(const Node* node, BigDVector<Node*>& parent)
{
    if (node != parent[node->point.x])
    {
        parent[node->point.x] = findSet(parent[node->point.x], parent);
    }
    return parent[node->point.x];
}

// Union of two sets
inline void unionSets(Node* x, Node* y, BigDVector<Node*>& parent)
{
    Node* px = findSet(x, parent);
    Node* py = findSet(y, parent);
    *parent.GetData(px->point.x) = py;
}

// Partition function for QuickSort
inline int partition(BigDVector<Edge>& edges, int low, int high)
{
    Edge* pivot = &edges[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (edges[j].weight <= pivot->weight)
        {
            i++;
            Edge temp = edges[i];
            edges[i] = edges[j];
            edges[j] = temp;
        }
    }

    Edge temp = edges[i + 1];
    edges[i + 1] = edges[high];
    edges[high] = temp;

    return i + 1;
}

inline void quickSort(BigDVector<Edge>& edges, int low, int high)
{
    if (low < high)
    {
        int pi = partition(edges, low, high);
        quickSort(edges, low, pi - 1);
        quickSort(edges, pi + 1, high);
    }
}

inline BigDVector<Connection> findMSTKruskal(BigDVector<Node>& graph)
{
    BigDVector<Connection> mst;
    BigDVector<Edge> edges;
    BigDVector<Node*> parent;

    // Initialize parent array for disjoint set
    for (int i = 0; i < graph.GetSize(); i++)
    {
        parent.Push(&graph[i]);
    }

    // Create edges from the graph
    for (int i = 0; i < graph.GetSize(); i++)
    {
        Node* currentNode = &graph[i];
        for (int j = 0; j < currentNode->connections.GetSize(); j++)
        {
            Connection* conn = &currentNode->connections[j];
            edges.Push(Edge(currentNode, conn->ptr, conn->distance));
        }
    }

    // Sort edges by weight using QuickSort
    quickSort(edges, 0, edges.GetSize() - 1);

    // Process edges in ascending order
    for (int i = 0; i < edges.GetSize(); i++)
    {
        Edge* currentEdge = &edges[i];

        Node* sourceRoot = findSet(currentEdge->source, parent);
        Node* destRoot = findSet(currentEdge->destination, parent);

        // If including this edge doesn't create a cycle
        if (sourceRoot != destRoot)
        {
            mst.Push(Connection(currentEdge->destination, currentEdge->weight));
            unionSets(sourceRoot, destRoot, parent);
        }
    }

    return mst;
}


#endif //KRUSKAL_H
