#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "../Utils/BigDVector.h"
#include <opencv4/opencv2/core/types.hpp>
#include <set>
#include "../Infrastructure/Node.h"

struct GraphDistanceSorter;

inline BigDVector<cv::Point> findShortestPathDijkstra(BigDVector<Node>& graph, int startNodeIndex, int destNodeIndex)
{
    GraphDistanceSorter sorter(graph);

    std::set<Node*, GraphDistanceSorter> unusedNodes(sorter);
    for (int i = 0; i < graph.GetSize(); ++i)
    {
        unusedNodes.insert(&graph[i]);
    }

    while (unusedNodes.size() > 0)
    {
        Node* currentNode = *unusedNodes.begin();
        if (currentNode->distance == -1)
        {
            return { };
        }
        if (currentNode == &graph[destNodeIndex])
        {
            break;
        }

        unusedNodes.erase(currentNode);
        //update distances of connected nodes
        for (Connection const& con : currentNode->connections)
        {
            /*here we could check if the element is really in unusedNodes (search, O(log n)), but this would
            actually take longer than calculating the new distance (O(1)), which will in this case always be greater
            than the old one, so the distance is never updated for nodes not in unusedNodes ()*/
            double newDistance = currentNode->distance + con.distance;

            if (newDistance < con.ptr->distance || con.ptr->distance == -1)
            {
                unusedNodes.erase(con.ptr);
                con.ptr->distance = newDistance;
                con.ptr->predecessor = currentNode;
                unusedNodes.insert(con.ptr);
            }
        }
    }

    //now trace back the path as a list of points
    BigDVector<cv::Point> points;
    Node* current = &graph[destNodeIndex];

    points.Push(current->point);
    while (current != &graph[startNodeIndex])
    {
        if (current->predecessor == nullptr)
        {
            return { };
        }

        current = current->predecessor;
        points.Push(current->point);
    }

    return points;
}

#endif //DIJKSTRA_H
