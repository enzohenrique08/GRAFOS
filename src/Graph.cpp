#include "Graph.h"

Graph::Graph(int n) : n(n), adj(n) {}

void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}
