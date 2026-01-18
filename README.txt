INSTRUÇÕES DE COMPILAÇÃO E EXECUÇÃO

Disciplina: DCC059 – Teoria dos Grafos (UFJF) Problema: Defective Coloring (Coloração d-defeituosa) Linguagem: C++17

Compilação:
    make

Parâmetro:
    d = número máximo de vizinhos com a mesma cor

Execução Guloso:
    ./defective <instancia> <d> guloso [seed]
    Ex: ./defective instances/dsjc250.5.col.txt 2 guloso

Execução Randomizado:
    ./defective <instancia> <d> randomizado <alpha> <iteracoes> [seed]
    Ex: ./defective instances/dsjc250.5.col.txt 2 randomizado 0.3 50

Execução Reativo:
    ./defective <instancia> <d> reativo <lista_alphas> <iteracoes> <bloco> [seed]
    Ex: ./defective instances/dsjc250.5.col.txt 2 reativo 0.1,0.2,0.3,0.5 500 50