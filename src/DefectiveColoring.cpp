#include "DefectiveColoring.h"
#include <algorithm>
#include <numeric>
#include <climits>

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

// Guloso Randomizado
std::vector<int> DefectiveColoring::greedyRandomized(
    const Graph& g,
    int d,
    double alpha,
    std::mt19937& rng
) {
    int n = g.n;
    std::vector<int> color(n, -1);

    std::vector<int> candidates(n);
    std::iota(candidates.begin(), candidates.end(), 0);

    int maxColor = 0;

    while (!candidates.empty()) {
        // Ordena por grau (critério guloso)
        std::sort(candidates.begin(), candidates.end(),
                  [&](int a, int b) {
                      return g.adj[a].size() > g.adj[b].size();
                  });

        int lrcSize = std::max(1, (int)(alpha * candidates.size()));
        std::uniform_int_distribution<int> dist(0, lrcSize - 1);

        int idx = dist(rng);
        int v = candidates[idx];

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

    return color;
}

std::vector<int> DefectiveColoring::greedyRandomizedReactive(
    const Graph& g,
    int d,
    const std::vector<double>& alphas,
    int iterations,
    int blockSize,
    std::mt19937& rng
) {
    int k = alphas.size();

    std::vector<double> prob(k, 1.0 / k);
    std::vector<double> quality(k, 0.0);
    std::vector<int> count(k, 0);

    std::vector<int> bestSolution;
    int bestColors = INT_MAX;

    std::discrete_distribution<int> dist(prob.begin(), prob.end());

    for (int it = 1; it <= iterations; it++) {
        int idx = dist(rng);
        double alpha = alphas[idx];

        auto sol = greedyRandomized(g, d, alpha, rng);

        int colors = 0;
        for (int c : sol)
            colors = std::max(colors, c + 1);

        if (colors < bestColors) {
            bestColors = colors;
            bestSolution = sol;
        }

        quality[idx] += 1.0 / colors;
        count[idx]++;

        // Atualiza probabilidades ao final do bloco
        if (it % blockSize == 0) {
            double sum = 0.0;
            for (int i = 0; i < k; i++) {
                if (count[i] > 0)
                    prob[i] = quality[i] / count[i];
                sum += prob[i];
            }

            for (int i = 0; i < k; i++)
                prob[i] /= sum;

            std::fill(quality.begin(), quality.end(), 0.0);
            std::fill(count.begin(), count.end(), 0);
            dist = std::discrete_distribution<int>(prob.begin(), prob.end());
        }
    }

    return bestSolution;
}

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
