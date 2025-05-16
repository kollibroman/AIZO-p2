#ifndef CONNECTION_H
#define CONNECTION_H
#include "Node.h"
#include "Node.h"

class Connection
{
public:
    Connection() : ptr(nullptr), distance(1)
    {
    };

    Connection(Node* ptr, double const distance) : ptr(ptr), distance(distance)
    {
    };

    bool operator==(const Connection &other) const;
    Node* ptr;
    double distance;
};

inline bool Connection::operator==(const Connection &other) const
{
    return ptr == other.ptr && distance == other.distance;
}

#endif //CONNECTION_H
