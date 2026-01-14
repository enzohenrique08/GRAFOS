#ifndef DEFECTIVE_COLORING_H
#define DEFECTIVE_COLORING_H

#include "Graph.h"
#include <vector>

class DefectiveColoring {
public:
    static std::vector<int> greedy(const Graph& g, int d);
    static bool canUseColor(const Graph& g,
                            const std::vector<int>& color,
                            int v,
                            int c,
                            int d);
};

#endif
