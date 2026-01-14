#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph {
public:
    int n;
    std::vector<std::vector<int>> adj;

    Graph(int n = 0);
    void addEdge(int u, int v);
};

#endif
