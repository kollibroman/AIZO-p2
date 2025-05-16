#ifndef FORDFULKERSON_H
#define FORDFULKERSON_H

#include "../Infrastructure/Node.h"
#include "../Utils/GigaKomarVector.h"

// Structure to store flow information for each edge
struct FlowEdge
{
    Node* from;
    Node* to;
    double capacity;
    double flow;

    FlowEdge(Node* f, Node* t, double c)
        : from(f), to(t), capacity(c), flow(0) {}
};

// Structure to keep track of residual graph edges
class ResidualGraph {
private:
    GigaKomarVector<FlowEdge> edges;

public:
    void addEdge(Node* from, Node* to, double capacity)
    {
        edges.Push(FlowEdge(from, to, capacity));
        edges.Push(FlowEdge(to, from, 0)); // Reverse edge for residual flow
    }

    FlowEdge* getEdge(Node* from, Node* to)
    {
        for (int i = 0; i < edges.GetSize(); i++)
        {
            FlowEdge* edge = edges.GetData(i);
            if (edge->from == from && edge->to == to)
            {
                return edge;
            }
        }
        return nullptr;
    }

    double getResidualCapacity(Node* from, Node* to)
    {
        FlowEdge* forward = getEdge(from, to);
        if (forward)
        {
            return forward->capacity - forward->flow;
        }
        return 0;
    }

    void updateFlow(Node* from, Node* to, double additionalFlow)
    {
        FlowEdge* forward = getEdge(from, to);
        FlowEdge* backward = getEdge(to, from);

        if (forward && backward) {
            forward->flow += additionalFlow;
            backward->flow -= additionalFlow;
        }
    }
};

// Finds a path from source to sink using BFS
bool findPath(GigaKomarVector<Node>& graph, ResidualGraph& residual,
             Node* source, Node* sink, GigaKomarVector<Node*>& path)
{

    // Reset all nodes
    for (int i = 0; i < graph.GetSize(); i++)
    {
        Node* node = graph.GetData(i);
        node->predecessor = nullptr;
        node->distance = -1; // Using distance as visited flag
    }

    // Initialize BFS
    GigaKomarVector<Node*> queue;
    source->distance = 0;
    queue.Push(source);

    // BFS to find a path
    while (queue.GetSize() > 0)
    {
        Node* current = queue.GetData(0);

        // Remove first element (dequeue)
        for (int i = 1; i < queue.GetSize(); i++)
        {
            *queue.GetData(i - 1) = *queue.GetData(i);
        }
        queue.PopData();

        // Check all neighbors
        for (int i = 0; i < current->connections.GetSize(); i++)
        {
            Connection* conn = current->connections.GetData(i);
            Node* next = conn->ptr;

            if (next->distance == -1 && residual.getResidualCapacity(current, next) > 0)
            {
                next->predecessor = current;
                next->distance = current->distance + 1;
                queue.Push(next);

                if (next == sink) {
                    // Construct path
                    Node* pathNode = sink;
                    while (pathNode != nullptr) {
                        path.Push(pathNode);
                        pathNode = pathNode->predecessor;
                    }
                    return true;
                }
            }
        }
    }

    return false;
}

// Main Ford-Fulkerson algorithm
inline double findMaxFlow(GigaKomarVector<Node>& graph, int sourceIndex, int sinkIndex)
{
    Node* source = graph.GetData(sourceIndex);
    Node* sink = graph.GetData(sinkIndex);

    // Create residual graph
    ResidualGraph residual;
    for (int i = 0; i < graph.GetSize(); i++)
    {
        Node* node = graph.GetData(i);
        for (int j = 0; j < node->connections.GetSize(); j++)
        {
            Connection* conn = node->connections.GetData(j);
            residual.addEdge(node, conn->ptr, conn->distance);
        }
    }

    double maxFlow = 0;
    GigaKomarVector<Node*> path;

    // While there exists an augmenting path
    while (findPath(graph, residual, source, sink, path))
    {
        // Find minimum residual capacity along the path
        double pathFlow = 999999.0;
        for (int i = path.GetSize() - 1; i > 0; i--)
        {
            Node* current = path.GetData(i);
            Node* next = path.GetData(i - 1);
            double residualCap = residual.getResidualCapacity(current, next);

            if (residualCap < pathFlow)
            {
                pathFlow = residualCap;
            }
        }

        // Update residual capacities
        for (int i = path.GetSize() - 1; i > 0; i--)
        {
            Node* current = path.GetData(i);
            Node* next = path.GetData(i - 1);
            residual.updateFlow(current, next, pathFlow);
        }

        maxFlow += pathFlow;

        // Clear path for next iteration
        while (path.GetSize() > 0)
        {
            path.PopData();
        }
    }

    return maxFlow;
}

#endif //FORDFULKERSON_H
