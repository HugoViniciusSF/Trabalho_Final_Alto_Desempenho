# Resultados da Execução CUDA

## Comando Utilizado

```bash
./kmeans_cuda
```

## Saída do Terminal

```text
Carregou 333 pontos com 4 features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Iniciando K-Means CUDA
K-means concluido com sucesso em 10 interacoes
Tempo total: 0.158066884 segundos
Tempo dos kernels: 0.001422059 segundos
Tempo de transferencia CPU-GPU: 0.156229382 segundos
Clusters salvo: ../../data/processed/results_cuda.csv
```

## Métricas

| Métrica | Valor |
|---|---:|
| Tempo sequencial | 0.000080000 s |
| Tempo CUDA | 0.158066884 s |
| Tempo dos kernels | 0.001422059 s |
| Tempo de transferência CPU-GPU | 0.156229382 s |
| Iterações | 10 |
| Processos/threads usados | 512 threads (2 blocos de 256) |
| Speedup | 0.000506 |
| Eficiência | 0.000000988 |

## Observações

- Comparação dos clusters gerados com o sequencial: Os agrupamentos resultantes são 99,7% idênticos aos da versão de referência na CPU. A verificação indicou que apenas um único ponto (ID 328) foi atribuído a um cluster diferente. Esta ligeira divergência é um comportamento esperado, justificado pela diferença na precisão aritmética de ponto flutuante entre a CPU e a GPU durante o cálculo iterativo da distância. É devido a este ponto de fronteira que a GPU necessitou de 10 iterações para convergir, contra as 9 da versão sequencial.

- Participação do tempo de kernels e transferência: O tempo total foi fortemente dominado pela comunicação. A transferência de memória e a inicialização do contexto da GPU representaram aproximadamente 98,8% do tempo total (0.156s), enquanto o processamento matemático nos kernels CUDA consumiu apenas 0,9% (0.0014s). Isto comprova que, para datasets minúsculos (333 linhas), o custo de aceder ao barramento PCIe anula os benefícios do paralelismo massivo. A eficiência do hardware atingiu praticamente 0%, de acordo com a Lei de Amdahl.

- Comparação com a versão OpenMP GPU: A versão OpenMP GPU apresentou tempo total menor que CUDA (0.125462111s contra 0.158066884s), mas o tempo de kernels foi maior (0.004286660s contra 0.001422059s). Isso indica que CUDA foi mais eficiente na computação dos kernels, enquanto OpenMP GPU teve menor custo total de execução neste teste. Os clusters gerados pelas duas versões GPU foram idênticos.

- Otimização de arquitetura: Para reduzir o custo de comunicação, a versão CUDA atualiza os centroides diretamente na GPU. O uso de `atomicAdd` evita cópias intermediárias do dataset e das labels a cada iteração, restringindo o tráfego no barramento principalmente à verificação do critério de parada.
