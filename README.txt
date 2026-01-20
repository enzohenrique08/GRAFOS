INSTRUÇÕES DE COMPILAÇÃO E EXECUÇÃO

Disciplina: DCC059 – Teoria dos Grafos (UFJF) Problema: Defective Coloring (Coloração d-defeituosa) Linguagem: C++17

Compilação:
    make

Execução Guloso:
    ./defective <instancia> <d> guloso [seed]
    Ex: ./defective instances/dsjc250.5.col.txt 2 guloso

Execução Randomizado:
    ./defective <instancia> <d> randomizado <alpha> <iteracoes> [seed]
    Ex: ./defective instances/dsjc250.5.col.txt 2 randomizado 0.2 50

Execução Reativo:
    ./defective <instancia> <d> reativo <lista_alphas> <iteracoes> <bloco> [seed]
    Ex: ./defective instances/dsjc250.5.col.txt 2 reativo 0.2,0.4,0.9 500 50

Parâmetro:
    d = número máximo de vizinhos com a mesma cor
    alpha = controla o grau de aleatoriedade na escolha do próximo vértice a ser colorido
    seed = é o valor inicial utilizado para "semear" o gerador de números pseudoaleatórios
    iterações = define quantas vezes o algoritmo vai tentar construir uma solução completa do zero dentro de uma única chamada
    bloco = determina a cada quantas iterações o algoritmo deve parar, analisar os resultados recentes e recalcular as probabilidades de escolha dos alphas

Ordem dos dados em results.csv:
    data/hora do teste, instâncias, algoritmo executado no teste, d, alpha, qntd de iterações, qntd de blocos, seed, tempo, melhor solução
    Ex.: 2026-01-18 21:59:19,instances/dsjc1000.1.col.txt,reativo,2,0.4,500,50,1003419234,2.42211,15

Ordem dos dados em resumo_final.csv:
    instância, algoritmo executado no teste, d, melhor solução, média das soluções, média dos tempos de execução
    Ex.: instances/dsjc500.9.col.txt,reativo,2,82,82.8,1.49315