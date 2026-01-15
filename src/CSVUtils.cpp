#include "CSVUtils.h"
#include <fstream>
#include <chrono>
#include <iomanip>

void appendCSV(
    const std::string& instancia,
    const std::string& algoritmo,
    int d,
    double alpha,
    int iteracoes,
    int bloco,
    unsigned seed,
    double tempo,
    int cores
) {
    std::ofstream file("results.csv", std::ios::app);

    // data/hora atual
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    file << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << ",";
    file << instancia << ",";
    file << algoritmo << ",";
    file << d << ",";
    
    if (alpha >= 0)
        file << alpha;
    file << ",";

    if (iteracoes >= 0)
        file << iteracoes;
    file << ",";

    if (bloco >= 0)
        file << bloco;
    file << ",";

    file << seed << ",";
    file << tempo << ",";
    file << cores << "\n";

    file.close();
}
