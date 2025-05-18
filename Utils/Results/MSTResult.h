#ifndef MSTRESULT_H
#define MSTRESULT_H
#include "../DefinitelyNotADataStructures/DefinitelyNotATuple.h"
#include "../DefinitelyNotADataStructures/DefinitelyNotAVector.h"

struct MSTResult {
    DefinitelyNotAVector<DefinitelyNotATuple<int, int, int>> edges; // (from, to, weight)
    int totalWeight;

    MSTResult() : totalWeight(0) {}
};


#endif //MSTRESULT_H
