#ifndef NODE_H
#define NODE_H

#include <opencv4/opencv2/core/types.hpp>
#include "Connection.h"
#include "../Utils/BigDVector.h"

class Node
{
public:
    Node() : distance(-1), predecessor(nullptr), connections(8)
    {
    };

    cv::Point point
    {
    };

    double distance;
    Node* predecessor;
    BigDVector<Connection> connections;

    bool operator==(const Node &lhs, const Node &rhs) const
    {
        return lhs.point == rhs.point;
    }
};

#endif //NODE_H
