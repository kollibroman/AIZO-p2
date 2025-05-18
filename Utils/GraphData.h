#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include "DefinitelyNotADataStructures/DefinitelyNotATuple.h"
#include "DefinitelyNotADataStructures/DefinitelyNotAVector.h"

struct GraphData {
    int numEdges;
    int numVertices;
    int startVertex;
    int endVertex;
    DefinitelyNotAVector<DefinitelyNotATuple<int, int, int>> edges; // <start_vertex, end_vertex, weight/capacity>
};

#endif //GRAPHDATA_H
