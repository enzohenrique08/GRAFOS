#ifndef DEFECTIVE_COLORING_H
#define DEFECTIVE_COLORING_H

#include "Graph.h"
#include <vector>
#include <random>

class DefectiveColoring {
public:
    // =====================================================
    // Guloso puro (determinístico)
    // =====================================================
    static std::vector<int> greedy(
        const Graph& g,
        int d
    );

    // =====================================================
    // Guloso randomizado
    // alpha      -> parâmetro de aleatoriedade
    // iterations -> número de execuções
    // rng        -> gerador com seed controlada
    // =====================================================
    static std::vector<int> greedyRandomized(
        const Graph& g,
        int d,
        double alpha,
        int iterations,
        std::mt19937& rng
    );

    // =====================================================
    // Guloso randomizado reativo
    // alphas     -> conjunto de valores de alpha
    // iterations -> número total de iterações
    // blockSize  -> tamanho do bloco de atualização
    // rng        -> gerador com seed controlada
    // =====================================================
    static std::vector<int> greedyRandomizedReactive(
        const Graph& g,
        int d,
        const std::vector<double>& alphas,
        int iterations,
        int blockSize,
        std::mt19937& rng
    );

private:
    // Verifica se o vértice v pode receber a cor c
    // respeitando o defeito máximo d
    static bool canUseColor(
        const Graph& g,
        const std::vector<int>& color,
        int v,
        int c,
        int d
    );
};

#endif
