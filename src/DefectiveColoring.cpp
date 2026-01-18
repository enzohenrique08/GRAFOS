#include "DefectiveColoring.h"
#include <algorithm>
#include <numeric>
#include <climits>

/* =====================================================
   GULOSO PURO
   ===================================================== */
std::vector<int> DefectiveColoring::greedy(const Graph& g, int d) {
    int n = g.n;
    std::vector<int> color(n, -1);

    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);

    std::sort(order.begin(), order.end(),
              [&](int a, int b) {
                  return g.adj[a].size() > g.adj[b].size();
              });

    int maxColor = 0;

    for (int v : order) {
        bool assigned = false;

        for (int c = 0; c < maxColor; c++) {
            if (canUseColor(g, color, v, c, d)) {
                color[v] = c;
                assigned = true;
                break;
            }
        }

        if (!assigned) {
            color[v] = maxColor++;
        }
    }

    return color;
}

/* =====================================================
   GULOSO RANDOMIZADO COM ITERAÇÕES (OBRIGATÓRIO NO TRABALHO)
   ===================================================== */
std::vector<int> DefectiveColoring::greedyRandomized(
    const Graph& g,
    int d,
    double alpha,
    int iterations,
    std::mt19937& rng
) {
    std::vector<int> bestSolution;
    int bestColors = INT_MAX;

    for (int it = 0; it < iterations; it++) {

        int n = g.n;
        std::vector<int> color(n, -1);

        std::vector<int> candidates(n);
        std::iota(candidates.begin(), candidates.end(), 0);

        int maxColor = 0;

        while (!candidates.empty()) {

            std::sort(candidates.begin(), candidates.end(),
                      [&](int a, int b) {
                          return g.adj[a].size() > g.adj[b].size();
                      });

            int lrcSize = std::max(1, (int)(alpha * candidates.size()));
            std::uniform_int_distribution<int> dist(0, lrcSize - 1);

            int v = candidates[dist(rng)];

            bool assigned = false;
            for (int c = 0; c < maxColor; c++) {
                if (canUseColor(g, color, v, c, d)) {
                    color[v] = c;
                    assigned = true;
                    break;
                }
            }

            if (!assigned) {
                color[v] = maxColor++;
            }

            candidates.erase(
                std::remove(candidates.begin(), candidates.end(), v),
                candidates.end()
            );
        }

        int usedColors = 0;
        for (int c : color)
            usedColors = std::max(usedColors, c + 1);

        if (usedColors < bestColors) {
            bestColors = usedColors;
            bestSolution = color;
        }
    }

    return bestSolution;
}

/* =====================================================
   GULOSO RANDOMIZADO REATIVO
   ===================================================== */
std::vector<int> DefectiveColoring::greedyRandomizedReactive(
    const Graph& g, int d, const std::vector<double>& alphas, 
    int iterations, int blockSize, std::mt19937& rng, double& bestAlphaFound) 
{
    int k = alphas.size();
    std::vector<double> prob(k, 1.0 / k);
    std::vector<double> sumSolutions(k, 0.0);
    std::vector<int> count(k, 0);

    std::vector<int> bestSolution;
    int bestColors = INT_MAX;
    bestAlphaFound = -1.0; // Inicializa o rastreio

    for (int it = 1; it <= iterations; it++) {
        std::discrete_distribution<int> dist(prob.begin(), prob.end());
        int idx = dist(rng);
        double alpha = alphas[idx];

        auto sol = greedyRandomized(g, d, alpha, 1, rng);
        int colors = 0;
        for (int c : sol) colors = std::max(colors, c + 1);

        // Se encontrou uma solução melhor, guarda a solução e o alpha usado
        if (colors < bestColors) {
            bestColors = colors;
            bestSolution = sol;
            bestAlphaFound = alpha; // Registra o melhor alpha 
        }

        sumSolutions[idx] += colors;
        count[idx]++;

        if (it % blockSize == 0) { // Atualização das probabilidades ao fim do bloco [cite: 48]
            std::vector<double> q(k);
            double sumQ = 0.0;
            for (int i = 0; i < k; i++) {
                if (count[i] > 0) {
                    double avg = sumSolutions[i] / count[i];
                    q[i] = std::pow((double)bestColors / avg, 10);
                } else q[i] = 0.0;
                sumQ += q[i];
            }
            for (int i = 0; i < k; i++) prob[i] = q[i] / sumQ;
            std::fill(sumSolutions.begin(), sumSolutions.end(), 0.0);
            std::fill(count.begin(), count.end(), 0);
        }
    }
    return bestSolution;
}

/* =====================================================
   VERIFICAÇÃO DE COR (DEFECTIVE)
   ===================================================== */
bool DefectiveColoring::canUseColor(
    const Graph& g,
    const std::vector<int>& color,
    int v,
    int c,
    int d
) {
    int cnt = 0;
    for (int u : g.adj[v]) {
        if (color[u] == c && ++cnt > d)
            return false;
    }
    return true;
}
