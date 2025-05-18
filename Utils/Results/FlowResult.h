#ifndef FLOWRESULT_H
#define FLOWRESULT_H
#include "../DefinitelyNotADataStructures/DefinitelyNotATuple.h"
#include "../DefinitelyNotADataStructures/DefinitelyNotAVector.h"

struct FlowResult {
    int maxFlow;
    DefinitelyNotAVector<DefinitelyNotATuple<int, int, int>> flowEdges; // (from, to, flow)

    FlowResult() : maxFlow(0) {}
};


#endif //FLOWRESULT_H
