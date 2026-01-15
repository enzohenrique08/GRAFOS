#include "Graph.h"
#include "DefectiveColoring.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <random>
#include <vector>
#include "CSVUtils.h"

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
    if (argc < 4) {
        std::cout << "Uso:\n";
        std::cout << "./defective <instancia.col> <d> <algoritmo> [parametros] [seed]\n\n";
        std::cout << "Algoritmos disponiveis:\n";
        std::cout << "  guloso\n";
        std::cout << "  randomizado <alpha>\n";
        std::cout << "  reativo\n";
        return 0;
    }

    std::string instance = argv[1];
    int d = std::stoi(argv[2]);
    std::string algoritmo = argv[3];

    // Seed única
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    if (algoritmo == "randomizado" && argc >= 6) {
        seed = std::stoul(argv[5]);
    }
    else if (algoritmo == "reativo" && argc >= 5) {
        seed = std::stoul(argv[4]);
    }

    std::mt19937 rng(seed);
    std::cout << "Seed usada: " << seed << "\n";

    Graph g = readDIMACS(instance);

    std::vector<int> color;

    auto start = std::chrono::high_resolution_clock::now();

    if (algoritmo == "guloso") {
        color = DefectiveColoring::greedy(g, d);
    }
    else if (algoritmo == "randomizado") {
        if (argc < 5) {
            std::cerr << "Erro: informe o valor de alpha\n";
            return 1;
        }
        double alpha = std::stod(argv[4]);
        color = DefectiveColoring::greedyRandomized(g, d, alpha, rng);
    }
    else if (algoritmo == "reativo") {
        // Parâmetros padrão do reativo
        std::vector<double> alphas = {0.1, 0.3, 0.5};
        int iterations = 100;
        int blockSize = 10;

        color = DefectiveColoring::greedyRandomizedReactive(
            g, d, alphas, iterations, blockSize, rng
        );
    }
    else {
        std::cerr << "Algoritmo invalido. Use: guloso | randomizado | reativo\n";
        return 1;
    }

    auto end = std::chrono::high_resolution_clock::now();

    int numColors = 0;
    for (int c : color)
        numColors = std::max(numColors, c + 1);

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Cores usadas: " << numColors << "\n";
    std::cout << "Tempo (s): " << elapsed.count() << "\n";

    

    return 0;
}