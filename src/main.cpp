#include "Graph.h"
#include "DefectiveColoring.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>

Graph readDIMACS(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Erro ao abrir arquivo\n";
        exit(1);
    }

    std::string line;
    int n = 0, m = 0;
    Graph g;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == 'c') continue;

        if (line[0] == 'p') {
            std::stringstream ss(line);
            std::string tmp;
            ss >> tmp >> tmp >> n >> m;
            g = Graph(n);
        }
        else if (line[0] == 'e') {
            int u, v;
            char e;
            std::stringstream ss(line);
            ss >> e >> u >> v;
            g.addEdge(u - 1, v - 1);
        }
    }

    return g;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Uso: ./defective <instancia.col> <d>\n";
        return 0;
    }

    std::string instance = argv[1];
    int d = std::stoi(argv[2]);

    Graph g = readDIMACS(instance);

    auto start = std::chrono::high_resolution_clock::now();
    auto color = DefectiveColoring::greedy(g, d);
    auto end = std::chrono::high_resolution_clock::now();

    int numColors = 0;
    for (int c : color)
        numColors = std::max(numColors, c + 1);

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Cores usadas: " << numColors << "\n";
    std::cout << "Tempo (s): " << elapsed.count() << "\n";

    return 0;
}
