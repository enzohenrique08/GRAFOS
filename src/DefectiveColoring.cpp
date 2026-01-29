#include "DefectiveColoring.h"
#include <algorithm>
#include <numeric>
#include <climits>

// GULOSO PURO
//algoritmo determinístico. Ele ordena os vértices pelo grau (do maior para o menor) 
//e atribui a cada um a primeira cor disponível que não viole o limite $d$ de vizinhos coloridos
std::vector<int> DefectiveColoring::greedy(const Graph& g, int d) {
    int n = g.n; // pega a quantidade de vértices
    std::vector<int> color(n, -1); // inicializa os sem cores

    std::vector<int> order(n); // vetor de indices
    std::iota(order.begin(), order.end(), 0); // preenche ele iniciando do zero

    // ordena os vértices de forma decrescente pelo grau
    std::sort(order.begin(), order.end(),
              [&](int a, int b) {
                  return g.adj[a].size() > g.adj[b].size();
              });

    int maxColor = 0; // cont para cores criadas

    for (int v : order) { // for de maior grau
        bool assigned = false;

        for (int c = 0; c < maxColor; c++) { // tenta as cores existentes
            if (canUseColor(g, color, v, c, d)) { // limitador
                color[v] = c; // atribui a cor
                assigned = true;
                break;
            }
        }

        if (!assigned) { // se precisar criar outra cor
            color[v] = maxColor++;
        }
    }

    return color;
}

// GULOSO RANDOMIZADO COM ITERAÇÕES
//Introduz aleatoriedade através de um parâmetro alpha. 
//Em vez de pegar sempre o maior grau, ele sorteia um vértice dentro de uma LRC composta pelos top alpha melhores
std::vector<int> DefectiveColoring::greedyRandomized(
    const Graph& g,
    int d,
    double alpha,
    int iterations,
    std::mt19937& rng
) {
    std::vector<int> bestSolution; // armazenar melhor solução entre as iterações
    int bestColors = INT_MAX; // valor finito inteiro

    for (int it = 0; it < iterations; it++) { // iterações

        int n = g.n;
        std::vector<int> color(n, -1); // vetor de cores

        std::vector<int> candidates(n); 
        std::iota(candidates.begin(), candidates.end(), 0); // preenche lista de candidatos dos vértices

        int maxColor = 0; // cont de cores

        while (!candidates.empty()) { // enquanto tiver candidatos
            
            //ordena candidatos restante pelo grau
            std::sort(candidates.begin(), candidates.end(),
                      [&](int a, int b) {
                          return g.adj[a].size() > g.adj[b].size();
                      });

            //caucula lrc baseada no alpha
            int lrcSize = std::max(1, (int)(alpha * candidates.size()));
            std::uniform_int_distribution<int> dist(0, lrcSize - 1); // sorteio entre os melhores candidatos

            int v = candidates[dist(rng)]; // sorteia vertice da LRC

            bool assigned = false;
            for (int c = 0; c < maxColor; c++) { // tenta cores existentes
                if (canUseColor(g, color, v, c, d)) { //checa se o defeito 'd' é respeitado
                    color[v] = c;
                    assigned = true;
                    break;
                }
            }

            if (!assigned) {
                color[v] = maxColor++; // cria nova cor
            }

            candidates.erase( // atualiza lista de candidatos
                std::remove(candidates.begin(), candidates.end(), v),
                candidates.end()
            );
        }

        int usedColors = 0; 
        for (int c : color)// quantas cores foram usada na iteração
            usedColors = std::max(usedColors, c + 1);
        
        //atualiza melhor solução encontrada
        if (usedColors < bestColors) {
            bestColors = usedColors;
            bestSolution = color;
        }
    }

    return bestSolution;
}

// GULOSO RANDOMIZADO REATIVO
//Ele testa vários valores de alpha e, periodicamente, 
//aumenta a probabilidade de escolher os valores de alpha que geraram as melhores soluções nos blocos anteriores
std::vector<int> DefectiveColoring::greedyRandomizedReactive(
    const Graph& g, int d, const std::vector<double>& alphas, 
    int iterations, int blockSize, std::mt19937& rng, double& bestAlphaFound) 
{
    int k = alphas.size();
    std::vector<double> prob(k, 1.0 / k); //mesma probabilidade para os alphas
    std::vector<double> sumSolutions(k, 0.0); // soma das cores obtidas por cada alpha
    std::vector<int> count(k, 0); // cont de cada alpha

    std::vector<int> bestSolution;
    int bestColors = INT_MAX;
    bestAlphaFound = -1.0; // Inicializa o rastreio

    for (int it = 1; it <= iterations; it++) {
        std::discrete_distribution<int> dist(prob.begin(), prob.end()); // sorteia alpha das probabilidades atuais
        int idx = dist(rng);
        double alpha = alphas[idx];

        //executa o guloso randomizado com o alpha escolhido
        auto sol = greedyRandomized(g, d, alpha, 1, rng);
        int colors = 0;
        for (int c : sol) colors = std::max(colors, c + 1);

        // Se encontrou uma solução melhor, guarda a solução e o alpha usado
        if (colors < bestColors) {
            bestColors = colors;
            bestSolution = sol;
            bestAlphaFound = alpha;
        }

        sumSolutions[idx] += colors; //registra o desempenho
        count[idx]++; 

        if (it % blockSize == 0) { // Atualização das probabilidades ao fim do bloco
            std::vector<double> q(k); // vetor de qualidade
            double sumQ = 0.0;
            for (int i = 0; i < k; i++) {
                if (count[i] > 0) {
                    double avg = sumSolutions[i] / count[i]; // calcula média de cores do alpha
                    q[i] = std::pow((double)bestColors / avg, 10); // quanto menor a média em relação ao melhor, maior peso
                } else q[i] = 0.0;
                sumQ += q[i];
            }
            for (int i = 0; i < k; i++) prob[i] = q[i] / sumQ; // atualiza qualidade
            std::fill(sumSolutions.begin(), sumSolutions.end(), 0.0); // reseta soluções
            std::fill(count.begin(), count.end(), 0);
        }
    }
    return bestSolution;
}

// VERIFICAÇÃO DE COR (DEFECTIVE)
bool DefectiveColoring::canUseColor(
    const Graph& g,
    const std::vector<int>& color,
    int v,
    int c,
    int d
) {
    int cnt = 0; // cont conflitos
    for (int u : g.adj[v]) { // percorre vizinhos de v
        if (color[u] == c && ++cnt > d) // se vizinho ja tem a cor e isso ultrapassar o d
            return false;
    }
    return true; // valida cor
}
