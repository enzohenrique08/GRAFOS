#ifndef DEFECTIVE_COLORING_H
#define DEFECTIVE_COLORING_H

#include "Graph.h"
#include <vector>
#include <random>

class DefectiveColoring {
public:
    // Guloso puro
    static std::vector<int> greedy(const Graph& g, int d);

    // Guloso randomizado
    static std::vector<int> greedyRandomized(
        const Graph& g,
        int d,
        double alpha,
        std::mt19937& rng
    );

    // Guloso reativo
    static std::vector<int> greedyRandomizedReactive(
    const Graph& g,
    int d,
    const std::vector<double>& alphas,
    int iterations,
    int blockSize,
    std::mt19937& rng
);

private:
    static bool canUseColor(
        const Graph& g,
        const std::vector<int>& color,
        int v,
        int c,
        int d
    );
};

#endif
