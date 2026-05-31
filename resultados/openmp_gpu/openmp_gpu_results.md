# Resultados da Execucao OpenMP GPU

## Comando Utilizado

```bash
./kmeans_openmp_gpu
```

## Saida do Terminal

```text
Carregou <num_pontos> pontos com <num_features> features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Iniciando K-Means OpenMP GPU
K-means concluido com sucesso em <iteracoes> interacoes
Tempo total: <tempo_total> segundos
Tempo de offload/transferencia: <tempo_transferencia> segundos
Clusters salvo: ../../data/processed/results_openmp_gpu.csv
```

## Metricas

| Metrica | Valor |
|---|---:|
| Tempo sequencial | 0.000080000 s |
| Tempo OpenMP GPU |  |
| Tempo de offload/transferencia |  |
| Iteracoes |  |
| Processos/threads usados |  |
| Speedup |  |
| Eficiencia |  |

## Observacoes

- Comparar os clusters gerados com `data/processed/results_sequencial.csv`.
- Registrar o impacto da transferencia CPU-GPU no tempo total.
- Comparar o desempenho com a versao CUDA quando ela estiver pronta.
