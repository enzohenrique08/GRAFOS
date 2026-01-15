#ifndef CSV_UTILS_H
#define CSV_UTILS_H

#include <string>

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
);

#endif
