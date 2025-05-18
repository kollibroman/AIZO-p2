#ifndef PATHRESULT_H
#define PATHRESULT_H
#include "../DefinitelyNotADataStructures/DefinitelyNotAVector.h"

struct PathResult
{
    DefinitelyNotAVector<int> distances;
    DefinitelyNotAVector<int> predecessors;
    bool hasNegativeCycle;

    PathResult() : hasNegativeCycle(false) {}
};


#endif //PATHRESULT_H
