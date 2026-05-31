# Resultados da Execucao CUDA

## Comando Utilizado

```bash
./kmeans_cuda
```

## Saida do Terminal

```text
Carregou <num_pontos> pontos com <num_features> features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Iniciando K-Means CUDA
K-means concluido com sucesso em <iteracoes> interacoes
Tempo total: <tempo_total> segundos
Tempo dos kernels: <tempo_kernels> segundos
Tempo de transferencia CPU-GPU: <tempo_transferencia> segundos
Clusters salvo: ../../data/processed/results_cuda.csv
```

## Metricas

| Metrica | Valor |
|---|---:|
| Tempo sequencial | 0.000080000 s |
| Tempo CUDA |  |
| Tempo dos kernels |  |
| Tempo de transferencia CPU-GPU |  |
| Iteracoes |  |
| Processos/threads usados |  |
| Speedup |  |
| Eficiencia |  |

## Observacoes

- Comparar os clusters gerados com `data/processed/results_sequencial.csv`.
- Registrar a participacao do tempo de kernels e transferencia no tempo total.
- Comparar o desempenho com a versao OpenMP GPU.
