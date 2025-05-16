#ifndef EDGE_H
#define EDGE_H
#include "Node.h"

struct Edge
{
    Node* source;
    Node* destination;
    double weight;

    Edge(Node* src, Node* dest, double w) : source(src), destination(dest), weight(w)
    {
    }

    bool operator<(const Edge& other) const
    {
        return weight < other.weight;
    }
};
#endif //EDGE_H
