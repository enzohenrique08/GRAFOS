#include "Graph.h"
#include "DefectiveColoring.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include <iomanip> 
#include "CSVUtils.h"

// Leitura das instâncias
Graph readInstances(const std::string& filename) {
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
        std::cout << "Uso Modificado (Automático 10x):\n";
        std::cout << "./defective <instancia> <d> <algoritmo> [params...]\n";
        return 0;
    }

    std::string instance = argv[1];
    int d = std::stoi(argv[2]);
    std::string algoritmo = argv[3];

    Graph g = readInstances(instance);
    
    // Variáveis para estatísticas das 10 execuções
    int bestGlobalColors = 2147483647;
    double totalTime = 0.0;
    double totalColors = 0.0;
    int numRuns = 10; 

    // Vetor para guardar a melhor solução para impressão
    std::vector<int> bestGlobalSolution;

    std::cout << ">>> Rodando 10x para: " << algoritmo << " em " << instance << "\n";

    for (int run = 0; run < numRuns; run++) {
        
        unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 rng(seed);

        std::vector<int> color;
        double currentAlpha = -1.0; 
        
        auto start = std::chrono::high_resolution_clock::now();

        // Seleção do Algoritmo
        if (algoritmo == "guloso") {
            color = DefectiveColoring::greedy(g, d);
        }
        else if (algoritmo == "randomizado") {
            double alpha = std::stod(argv[4]);
            int iteracoes = std::stoi(argv[5]);
            color = DefectiveColoring::greedyRandomized(g, d, alpha, iteracoes, rng);
            currentAlpha = alpha;
        }
        else if (algoritmo == "reativo") {
            std::vector<double> alphas;
            std::stringstream ss(argv[4]);
            std::string token;
            while (std::getline(ss, token, ',')) {
                alphas.push_back(std::stod(token));
            }
            int iteracoes = std::stoi(argv[5]);
            int bloco = std::stoi(argv[6]);
            
            color = DefectiveColoring::greedyRandomizedReactive(g, d, alphas, iteracoes, bloco, rng, currentAlpha);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        int numColors = 0;
        for (int c : color) numColors = std::max(numColors, c + 1);

        // Atualização do melhor global
        if (numColors < bestGlobalColors) {
            bestGlobalColors = numColors;
            // Salva o vetor dessa solução vencedora
            bestGlobalSolution = color;
        }
        
        totalColors += numColors;
        totalTime += elapsed.count();

        int iter_csv = (algoritmo != "guloso") ? std::stoi(argv[5]) : -1;
        int bloco_csv = (algoritmo == "reativo") ? std::stoi(argv[6]) : -1;

        appendCSV(instance, algoritmo, d, currentAlpha, iter_csv, bloco_csv, seed, elapsed.count(), numColors);
        
        std::cout << "   Run " << run+1 << ": " << numColors << " cores (" << elapsed.count() << "s)\n";
    }

    double avgTime = totalTime / numRuns;
    double avgColors = totalColors / numRuns;

    // Salva melhor solução, média cores, média tempo no resumo_final.csv
    std::ofstream summaryFile("resumo_final.csv", std::ios::app);
    if (summaryFile) {
        summaryFile << instance << "," << algoritmo << "," << d << "," 
                    << bestGlobalColors << "," << avgColors << "," << avgTime << "\n";
    }

    std::cout << "--- FIM ---\n";
    std::cout << "Melhor: " << bestGlobalColors << " | Media Cores: " << avgColors << " | Media Tempo: " << avgTime << "s\n";

    /* =====================================================
       Imprime a melhor das 10 soluções para visualizar (formato vértice-cor)
       ===================================================== */
    std::cout << "\n--- Solucao (Formato p/ Visualizador - Melhor Run) ---\n";
    for (int i = 0; i < g.n; i++) {
        // Formato: Vértice Cor
        std::cout << i + 1 << " " << bestGlobalSolution[i] << "\n";
    }
    std::cout << "------------------------------------------------------\n";

    return 0;
}