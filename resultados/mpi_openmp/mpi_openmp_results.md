# Resultados da Execucao MPI + OpenMP

## Comando Utilizado

```bash
OMP_NUM_THREADS=<threads> mpirun -np <processos> ./kmeans_mpi_openmp
```

## Saida do Terminal

```text
Carregou <num_pontos> pontos com <num_features> features.
Processos MPI: <processos>
Threads OpenMP por processo: <threads>
Iniciando K-Means MPI + OpenMP
K-means concluido com sucesso em <iteracoes> interacoes
Wall Clock Time: <tempo> segundos
Clusters salvo: ../../data/processed/results_mpi_openmp.csv
```

## Metricas

| Metrica | Valor |
|---|---:|
| Tempo sequencial | 0.000080000 s |
| Tempo MPI + OpenMP |  |
| Processos MPI |  |
| Threads por processo |  |
| Processos/threads usados |  |
| Iteracoes |  |
| Speedup |  |
| Eficiencia |  |

## Observacoes

- Comparar os clusters gerados com `data/processed/results_sequencial.csv`.
- Registrar se houve diferenca no numero de iteracoes em relacao ao sequencial.
- Discutir overhead de comunicacao e balanceamento de carga.
