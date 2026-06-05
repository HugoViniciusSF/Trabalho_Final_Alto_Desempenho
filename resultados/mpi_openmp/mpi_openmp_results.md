# Resultados da Execução MPI + OpenMP

## Comando Utilizado

```bash
OMP_NUM_THREADS=<threads> mpirun -np <processos> ./kmeans_mpi_openmp
```

## Saída do Terminal

```text
Carregou <num_pontos> pontos com <num_features> features.
Processos MPI: <processos>
Threads OpenMP por processo: <threads>
Iniciando K-Means MPI + OpenMP
K-means concluido com sucesso em <iteracoes> interacoes
Wall Clock Time: <tempo> segundos
Clusters salvo: ../../data/processed/results_mpi_openmp.csv
```

## Métricas

| Métrica | Valor |
|---|---:|
| Tempo sequencial | 0.000080000 s |
| Tempo MPI + OpenMP |  |
| Processos MPI |  |
| Threads por processo |  |
| Processos/threads usados |  |
| Iterações |  |
| Speedup |  |
| Eficiência |  |

## Observações

- Comparar os clusters gerados com `data/processed/results_sequencial.csv`.
- Registrar se houve diferença no número de iterações em relação ao sequencial.
- Discutir overhead de comunicação e balanceamento de carga.
