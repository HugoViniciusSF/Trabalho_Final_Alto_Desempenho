# Comparativo Geral dos Resultados

Este arquivo consolida os resultados já obtidos para as versões sequencial, OpenMP GPU e CUDA do K-means. A comparação segue o escopo do projeto: tempo de execução, speedup, eficiência, impacto da transferência CPU-GPU e equivalência dos agrupamentos gerados.

As avaliações de escalabilidade forte e fraca, overhead de comunicação no MPI e balanceamento de carga dependem da implementação MPI + OpenMP e dos testes com variação de entrada, processos e threads.

## Resumo de Desempenho

| Versão | Modelo de execução | Pontos | Iterações | Tempo total (s) | Tempo de kernels/computação (s) | Transferência/offload (s) | Speedup | Eficiência |
|---|---|---:|---:|---:|---:|---:|---:|---|
| Sequencial | CPU | 333 | 9 | 0.000080000 | - | - | 1.00 | 1.00 |
| OpenMP GPU | GPU com OpenMP offload | 333 | 10 | 0.125462111 | 0.004286660 | 0.121175451 | 0.000638 | Não calculada |
| CUDA | GPU com CUDA | 333 | 10 | 0.158066884 | 0.001422059 | 0.156229382 | 0.000506 | 0.000000988 |

## Comparação dos Clusters

| Comparação | Pontos comparados | Pontos iguais | Pontos diferentes | Concordância | Divergências |
|---|---:|---:|---:|---:|---|
| Sequencial x OpenMP GPU | 333 | 332 | 1 | 99.70% | ID 328: cluster 2 -> cluster 1 |
| Sequencial x CUDA | 333 | 332 | 1 | 99.70% | ID 328: cluster 2 -> cluster 1 |
| CUDA x OpenMP GPU | 333 | 333 | 0 | 100.00% | Nenhuma |

## Observações

- A versão sequencial é a base de comparação do projeto.
- As versões GPU não foram mais rápidas neste dataset porque a entrada possui apenas 333 pontos.
- O tempo das versões GPU foi dominado pela transferência de dados e pelo custo de inicialização do contexto na GPU.
- A versão CUDA teve menor tempo de kernels que a OpenMP GPU, mas maior tempo total.
- CUDA e OpenMP GPU produziram exatamente os mesmos clusters entre si.
- A única diferença em relação ao sequencial ocorreu no ponto ID 328, o que indica alta estabilidade dos agrupamentos.

## Como Reproduzir a Comparação dos Clusters

Comparar sequencial com CUDA:

```bash
python3 scripts/compare_results.py data/processed/results_sequencial.csv data/processed/results_cuda.csv
```

Comparar sequencial com OpenMP GPU:

```bash
python3 scripts/compare_results.py data/processed/results_sequencial.csv data/processed/results_openmp_gpu.csv
```

Comparar CUDA com OpenMP GPU:

```bash
python3 scripts/compare_results.py data/processed/results_cuda.csv data/processed/results_openmp_gpu.csv
```
