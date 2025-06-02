#ifndef TYPE1GRAPHDATAHANDLER_H
#define TYPE1GRAPHDATAHANDLER_H

#include "../Enums/Algorithm.h"
#include "../Enums/GraphRepresentationType.h"

struct GraphDataHandler
{
    GraphDataHandler() : algorithm(), MeasuredTime(0), representation(), Density(0), VertexCount(0)
    {
    }

    GraphDataHandler(const Algorithm algorithm, const double measuredTime, const int density, const GraphRepresentationType representation, const int vertexCount)
        : algorithm(algorithm), MeasuredTime(measuredTime), representation(representation), Density(density), VertexCount(vertexCount)
    {
    }

    Algorithm algorithm;
    double MeasuredTime;
    GraphRepresentationType representation;
    int Density;
    int VertexCount;
};

#endif //TYPE1GRAPHDATAHANDLER_H