#ifndef FLOWRESULT_H
#define FLOWRESULT_H
#include "../DefinitelyNotADataStructures/DefinitelyNotAVector.h"

struct FlowResult {
    int maxFlow;
    // Residual graph representation
    DefinitelyNotAVector<DefinitelyNotAVector<int>> residualGraph;
    // Store the actual flow values
    DefinitelyNotAVector<DefinitelyNotAVector<int>> flowGraph;
};



#endif //FLOWRESULT_H
