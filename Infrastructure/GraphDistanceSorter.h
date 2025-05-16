#ifndef GRAPHDISTANCESORTER_H
#define GRAPHDISTANCESORTER_H
#include "Node.h"

struct GraphDistanceSorter
{
    bool operator() (const Node* lhs, const Node* rhs) const;
};

inline bool GraphDistanceSorter::operator() (const Node* lhs, const Node* rhs) const
{
    if (lhs->distance == rhs->distance)
    {
        return lhs < rhs;
    }

    if (lhs->distance != -1 && rhs->distance != -1)
    {
        if (lhs->distance != rhs->distance)
        {
            return lhs->distance < rhs->distance;
        }
    }
    else if (lhs->distance != -1 && rhs->distance == -1)
    {
        return true;
    }
    return false;

}

#endif //GRAPHDISTANCESORTER_H
