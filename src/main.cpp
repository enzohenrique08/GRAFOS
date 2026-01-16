#include "Graph.h"
#include "DefectiveColoring.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>

/* =====================================================
   LEITURA DIMACS
   ===================================================== */
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

/* =====================================================
   MAIN
   ===================================================== */
int main(int argc, char* argv[]) {

    if (argc < 4) {
        std::cout << "Uso:\n";
        std::cout << "./defective <instancia.col> <d> <algoritmo> [parametros] [seed]\n\n";
        std::cout << "Algoritmos:\n";
        std::cout << "  guloso [seed]\n";
        std::cout << "  randomizado <alpha> <iteracoes> [seed]\n";
        std::cout << "  reativo <alphas> <iteracoes> <bloco> [seed]\n";
        std::cout << "Exemplo reativo:\n";
        std::cout << "  ./defective grafo.col 2 reativo 0.1,0.3,0.5 100 10 123\n";
        return 0;
    }

    std::string instance = argv[1];
    int d = std::stoi(argv[2]);
    std::string algoritmo = argv[3];

    /* =====================================================
       SEED ÚNICA
       ===================================================== */
    unsigned seed;
    if (argc >= 5) {
        seed = std::stoul(argv[argc - 1]);
    } else {
        seed = std::chrono::system_clock::now().time_since_epoch().count();
    }

    std::mt19937 rng(seed);
    std::cout << "Seed usada: " << seed << "\n";

    Graph g = readDIMACS(instance);
    std::vector<int> color;

    auto start = std::chrono::high_resolution_clock::now();

    /* =====================================================
       GULOSO
       ===================================================== */
    if (algoritmo == "guloso") {
        color = DefectiveColoring::greedy(g, d);
    }

    /* =====================================================
       GULOSO RANDOMIZADO
       ===================================================== */
    else if (algoritmo == "randomizado") {

        if (argc < 6) {
            std::cerr << "Uso: randomizado <alpha> <iteracoes> [seed]\n";
            return 1;
        }

        double alpha = std::stod(argv[4]);
        int iteracoes = std::stoi(argv[5]);

        color = DefectiveColoring::greedyRandomized(
            g, d, alpha, iteracoes, rng
        );
    }

    /* =====================================================
       GULOSO RANDOMIZADO REATIVO
       ===================================================== */
    else if (algoritmo == "reativo") {

        if (argc < 7) {
            std::cerr << "Uso: reativo <alphas> <iteracoes> <bloco> [seed]\n";
            return 1;
        }

        /* Parse da lista de alphas */
        std::vector<double> alphas;
        std::stringstream ss(argv[4]);
        std::string token;
        while (std::getline(ss, token, ',')) {
            alphas.push_back(std::stod(token));
        }

        int iteracoes = std::stoi(argv[5]);
        int bloco = std::stoi(argv[6]);

        color = DefectiveColoring::greedyRandomizedReactive(
            g, d, alphas, iteracoes, bloco, rng
        );
    }

    else {
        std::cerr << "Algoritmo invalido\n";
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
