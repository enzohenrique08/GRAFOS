#include "Graph.h"
#include "DefectiveColoring.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include "CSVUtils.h"

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
    double bestAlphaFound = -1.0; // Variável para rastrear o melhor alpha no reativo

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

        color = DefectiveColoring::greedyRandomized(g, d, alpha, iteracoes, rng);
        bestAlphaFound = alpha; // No randomizado simples, o "melhor" é o único usado
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

        // A função atualizada agora recebe bestAlphaFound por referência
        color = DefectiveColoring::greedyRandomizedReactive(
            g, d, alphas, iteracoes, bloco, rng, bestAlphaFound
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

    /* =====================================================
       GRAVAÇÃO DOS RESULTADOS EM CSV
       ===================================================== */
    int iter_csv = (algoritmo != "guloso") ? std::stoi(argv[5]) : -1;
    int bloco_csv = (algoritmo == "reativo") ? std::stoi(argv[6]) : -1;

    // Se for reativo, passamos o bestAlphaFound capturado na função
    // Se for randomizado, o alpha passado via parâmetro
    // Se for guloso, passará -1.0
    double alpha_csv = (algoritmo == "guloso") ? -1.0 : bestAlphaFound;

    appendCSV(
        instance,      // Instância 
        algoritmo,     // Algoritmo executado 
        d,             // Parâmetro d (restrição de grau) 
        alpha_csv,     // Parâmetro alpha (melhor alpha no caso do reativo)
        iter_csv,      // Número de iterações 
        bloco_csv,     // Tamanho do bloco (reativo) 
        seed,          // Semente de randomização 
        elapsed.count(), // Tempo gasto (segundos) 
        numColors      // Valor da melhor solução alcançada 
    );

    /* =====================================================
       IMPRESSÃO DA SOLUÇÃO (Para visualização externa)
       ===================================================== */
    std::cout << "\n--- Solucao (Formato p/ Visualizador) ---\n";
    for (int i = 0; i < g.n; i++) {
        std::cout << i + 1 << " " << color[i] << "\n";
    }

    return 0;
}