# Comparativo Geral dos Resultados

Este arquivo consolida os resultados já obtidos para as versões sequencial, MPI + OpenMP, OpenMP GPU e CUDA do K-means. A comparação segue o escopo do projeto: tempo de execução, speedup, eficiência, impacto da transferência CPU-GPU e concordância dos agrupamentos gerados.

As métricas experimentais da versão MPI + OpenMP foram medidas com variação de processos e threads. A avaliação de escalabilidade fraca ainda depende de testes com diferentes tamanhos de entrada.

## Resumo de Desempenho

| Versão | Modelo de execução | Pontos | Iterações | Tempo total (s) | Tempo de kernels/computação (s) | Transferência/offload (s) | Speedup | Eficiência |
|---|---|---:|---:|---:|---:|---:|---:|---|
| Sequencial | CPU | 333 | 9 | 0.000080000 | - | - | 1.00 | 1.00 |
| MPI + OpenMP | CPU com processos e threads | 333 | 9 | 0.000046000 | 0.000046000 | - | 1.739130 | 1.739130 |
| OpenMP GPU | GPU com OpenMP offload | 333 | 10 | 0.125462111 | 0.004286660 | 0.121175451 | 0.000638 | Não medida |
| CUDA | GPU com CUDA | 333 | 10 | 0.158066884 | 0.001422059 | 0.156229382 | 0.000506 | 0.000000988 |

## Comparação dos Clusters

| Comparação | Pontos comparados | Pontos iguais | Pontos diferentes | Concordância | Divergências |
|---|---:|---:|---:|---:|---|
| Sequencial x MPI + OpenMP | 333 | 332 | 1 | 99.70% | ID 328: cluster 1 -> cluster 2 |
| Sequencial x OpenMP GPU | 333 | 333 | 0 | 100.00% | Nenhuma |
| Sequencial x CUDA | 333 | 333 | 0 | 100.00% | Nenhuma |
| CUDA x OpenMP GPU | 333 | 333 | 0 | 100.00% | Nenhuma |
| MPI + OpenMP x CUDA | 333 | 332 | 1 | 99.70% | ID 328: cluster 2 -> cluster 1 |
| MPI + OpenMP x OpenMP GPU | 333 | 332 | 1 | 99.70% | ID 328: cluster 2 -> cluster 1 |

## Observações

- A versão sequencial é a base de comparação do projeto.
- As versões GPU não foram mais rápidas neste dataset porque a entrada possui apenas 333 pontos.
- O tempo das versões GPU foi dominado pela transferência de dados e pelo custo de inicialização do contexto na GPU.
- A versão CUDA teve menor tempo de kernels que a OpenMP GPU, mas maior tempo total.
- Sequencial, CUDA e OpenMP GPU produziram os mesmos clusters para os 333 pontos do dataset tratado.
- A versão MPI + OpenMP divergiu em apenas um ponto, mantendo 99,70% de concordância.
- A melhor execução MPI + OpenMP foi com 1 processo MPI e 1 thread OpenMP.
- Nas configurações MPI + OpenMP com mais processos e threads, os tempos aumentaram em relação à execução com 1 processo e 1 thread.
- O dataset possui apenas 333 pontos, então os resultados devem ser interpretados considerando o tamanho reduzido da entrada.

## Como Reproduzir a Comparação dos Clusters

Comparar sequencial com CUDA:

```bash
python3 scripts/compare_results.py data/processed/results_sequencial.csv data/processed/results_cuda.csv
```

Comparar sequencial com MPI + OpenMP:

```bash
python3 scripts/compare_results.py data/processed/results_sequencial.csv data/processed/results_mpi_openmp.csv
```

Comparar sequencial com OpenMP GPU:

```bash
python3 scripts/compare_results.py data/processed/results_sequencial.csv data/processed/results_openmp_gpu.csv
```

Comparar CUDA com OpenMP GPU:

```bash
python3 scripts/compare_results.py data/processed/results_cuda.csv data/processed/results_openmp_gpu.csv
```

Comparar MPI + OpenMP com CUDA:

```bash
python3 scripts/compare_results.py data/processed/results_mpi_openmp.csv data/processed/results_cuda.csv
```

Comparar MPI + OpenMP com OpenMP GPU:

```bash
python3 scripts/compare_results.py data/processed/results_mpi_openmp.csv data/processed/results_openmp_gpu.csv
```
