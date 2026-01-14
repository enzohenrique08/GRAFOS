#include "DefectiveColoring.h"
#include <algorithm>

std::vector<int> DefectiveColoring::greedy(const Graph& g, int d) {
    int n = g.n;
    std::vector<int> color(n, -1);

    std::vector<int> order(n);
    for (int i = 0; i < n; i++) order[i] = i;

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
            color[v] = maxColor;
            maxColor++;
        }
    }

    return color;
}

bool DefectiveColoring::canUseColor(const Graph& g,
                                   const std::vector<int>& color,
                                   int v,
                                   int c,
                                   int d) {
    int cnt = 0;

    for (int u : g.adj[v]) {
        if (color[u] == c) cnt++;
        if (cnt > d) return false;
    }

    return true;
}
