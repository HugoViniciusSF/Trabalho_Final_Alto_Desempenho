# Resultados da Execução MPI + OpenMP

## Comando Utilizado

```bash
OMP_NUM_THREADS=<threads> mpirun -np <processos> ./kmeans_mpi_openmp
```

## Saída do Terminal

```text
Carregou 333 pontos com 4 features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Processos MPI: 1, 2 ou 4
Threads OpenMP por processo: 1, 2, 4 ou 8
Iniciando K-Means MPI + OpenMP
K-means concluido com sucesso em 9 interacoes
Wall Clock Time: valores registrados na tabela de métricas
Clusters salvo: ../../data/processed/results_mpi_openmp.csv
```

## Métricas

Tempo sequencial de referência: `0.000080000 s`.

| Processos MPI | Threads por processo | Total de unidades | Tempo (s) | Iterações | Speedup | Eficiência |
|---:|---:|---:|---:|---:|---:|---:|
| 1 | 1 | 1 | 0.000055000 | 9 | 1.454545 | 1.454545 |
| 1 | 2 | 2 | 0.000229000 | 9 | 0.349345 | 0.174672 |
| 1 | 4 | 4 | 0.000318000 | 9 | 0.251572 | 0.062893 |
| 1 | 8 | 8 | 0.000755000 | 9 | 0.105960 | 0.013245 |
| 2 | 1 | 2 | 0.000687000 | 9 | 0.116448 | 0.058224 |
| 2 | 2 | 4 | 0.000657000 | 9 | 0.121766 | 0.030441 |
| 2 | 4 | 8 | 0.001233000 | 9 | 0.064882 | 0.008110 |
| 2 | 8 | 16 | 0.001817000 | 9 | 0.044029 | 0.002752 |
| 4 | 1 | 4 | 0.000463000 | 9 | 0.172786 | 0.043197 |
| 4 | 2 | 8 | 0.001249000 | 9 | 0.064051 | 0.008006 |
| 4 | 4 | 16 | 0.001724000 | 9 | 0.046404 | 0.002900 |
| 4 | 8 | 32 | 0.002573000 | 9 | 0.031092 | 0.000972 |

## Comparação dos Clusters

O arquivo `data/processed/results_mpi_openmp.csv` possui 333 pontos, mesma quantidade dos arquivos das demais versões.

Comandos executados:

```bash
python3 scripts/compare_results.py data/processed/results_sequencial.csv data/processed/results_mpi_openmp.csv
python3 scripts/compare_results.py data/processed/results_mpi_openmp.csv data/processed/results_cuda.csv
python3 scripts/compare_results.py data/processed/results_mpi_openmp.csv data/processed/results_openmp_gpu.csv
```

| Comparação | Pontos comparados | Pontos iguais | Pontos diferentes | Concordância | Divergência |
|---|---:|---:|---:|---:|---|
| Sequencial x MPI + OpenMP | 333 | 332 | 1 | 99.70% | ID 328: cluster 1 -> cluster 2 |
| MPI + OpenMP x CUDA | 333 | 332 | 1 | 99.70% | ID 328: cluster 2 -> cluster 1 |
| MPI + OpenMP x OpenMP GPU | 333 | 332 | 1 | 99.70% | ID 328: cluster 2 -> cluster 1 |

## Observações

- A versão MPI + OpenMP gera clusters com alta concordância em relação às demais versões registradas.
- A única divergência ocorreu no ponto 328.
- A melhor medição foi com 1 processo MPI e 1 thread OpenMP, com tempo de `0.000055000 s`.
- O aumento de processos e threads não trouxe ganho para este dataset, pois a entrada possui apenas 333 pontos.
- Os tempos aumentaram nas configurações com mais processos e threads, indicando overhead de paralelização relevante para esta entrada.
- O balanceamento de carga não foi medido separadamente. A divisão dos pontos é feita de forma quase uniforme entre os processos.
- Como os tempos são da ordem de microssegundos, pequenas variações de medição podem afetar speedup e eficiência.
