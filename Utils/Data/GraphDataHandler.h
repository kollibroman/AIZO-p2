#ifndef TYPE1GRAPHDATAHANDLER_H
#define TYPE1GRAPHDATAHANDLER_H

#include "../Enums/Algorithm.h"
#include "../Enums/GraphRepresentationType.h"

struct GraphDataHandler
{
    GraphDataHandler() : algorithm(), MeasuredTime(0), representation(), Density(0)
    {
    }

    GraphDataHandler(const Algorithm algorithm, const long measuredTime, const int density, const GraphRepresentationType representation) : algorithm(algorithm),
        MeasuredTime(measuredTime), representation(representation), Density(density)
    {
    }

    Algorithm algorithm;
    long MeasuredTime;
    GraphRepresentationType representation;
    int Density;
};

#endif //TYPE1GRAPHDATAHANDLER_H
