#ifndef PRIM_H
#define PRIM_H

#include "../Infrastructure/Node.h"
#include "../Utils/GigaKomarVector.h"

class PrimNode
{
public:
    Node* node;
    double key;
    bool visited;

    PrimNode() : node(nullptr), key(999999.0), visited(false) {}

    explicit PrimNode(Node* n) : node(n), key(999999.0), visited(false) {}
};

inline int findMinKeyNode(GigaKomarVector<PrimNode>& nodes)
{
    double minKey = 999999.0;
    int minIndex = -1;

    for (int v = 0; v < nodes.GetSize(); v++)
    {
        PrimNode* current = &nodes[v];
        if (!current->visited && current->key < minKey)
        {
            minKey = current->key;
            minIndex = v;
        }
    }

    return minIndex;
}

inline GigaKomarVector<Connection> findMSTPrim(GigaKomarVector<Node>& graph, int startVertex)
{
    if (graph.GetSize() == 0)
    {
        return { };
    }

    // Initialize result MST
    GigaKomarVector<Connection> mst;

    // Create auxiliary array for Prim's algorithm
    GigaKomarVector<PrimNode> primNodes;

    // Initialize all vertices
    for (int i = 0; i < graph.GetSize(); i++)
    {
        primNodes.Push(PrimNode(graph[i]));
    }

    // Start with first vertex
    primNodes.GetData(startVertex)->key = 0;

    // Find MST with V vertices
    for (int i = 0; i < graph.GetSize(); i++)
    {
        // Find minimum key vertex
        int minIndex = findMinKeyNode(primNodes);
        if (minIndex == -1) {
            break;  // No more connected components
        }

        // Add chosen vertex to MST
        PrimNode* minNode = primNodes.GetData(minIndex);
        minNode->visited = true;

        // Add edge to MST if not the first vertex
        if (minNode->node->predecessor != nullptr)
        {
            mst.Push(Connection(minNode->node, minNode->key));
        }

        // Update key values of adjacent vertices
        Node* currentNode = minNode->node;
        for (int j = 0; j < currentNode->connections.GetSize(); j++)
        {
            Connection* conn = &currentNode->connections[j];

            // Find the PrimNode for this connection
            for (int k = 0; k < primNodes.GetSize(); k++)
            {
                PrimNode* adjacentPrim = &primNodes[k];

                if (adjacentPrim->node == conn->ptr &&
                    !adjacentPrim->visited &&
                    conn->distance < adjacentPrim->key)
                {

                    adjacentPrim->key = conn->distance;
                    adjacentPrim->node->predecessor = currentNode;
                }
            }
        }
    }

    return mst;
}

#endif //PRIM_H
