# Resultados da Execução OpenMP GPU

## Comando Utilizado

```bash
./kmeans_openmp_gpu
```

## Saída do Terminal

```text
Carregou 333 pontos com 4 features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Iniciando K-Means OpenMP GPU
K-means concluido com sucesso em 10 interacoes
Tempo total: 0.125462111 segundos
Tempo de offload/transferencia: 0.121175451 segundos
Clusters salvo: ../../data/processed/results_openmp_gpu.csv
```

## Métricas

| Métrica | Valor |
|---|---:|
| Tempo sequencial | 0.000080000 s |
| Tempo OpenMP GPU | 0.125462111 s |
| Tempo de offload/transferência | 0.121175451 s |
| Tempo de kernels (calculado) | 0.004286660 s |
| Iterações | 10 |
| Processos/threads usados | Automático pelo runtime OpenMP (nvptx-none, sm_80) |
| Speedup | 0.000638 |

## Observações

- Comparação dos clusters gerados com `data/processed/results_sequencial.csv`: Os agrupamentos resultantes são idênticos aos da versão CUDA e diferem do sequencial em exatamente um ponto (ID 328, atribuído ao cluster 1 em vez do cluster 2). Esta divergência mínima é esperada e decorre da diferença na ordem das operações atômicas de ponto flutuante em execução paralela, alterando levemente o cálculo dos centroides a cada iteração. É por este ponto de fronteira que ambas as versões GPU precisaram de 10 iterações para convergir, contra as 9 da versão sequencial.

- Participação do tempo de offload e transferência: O tempo total foi fortemente dominado pela comunicação CPU-GPU. A transferência de dados e a inicialização do contexto OpenMP na GPU representaram aproximadamente 96,6% do tempo total (0.1212s), enquanto a execução dos kernels de K-means consumiu apenas 3,4% (0.0043s). Isso confirma, em linha com o que foi observado na versão CUDA, que para datasets minúsculos como o Penguins (333 pontos), o custo de acesso ao barramento PCIe e de inicialização do contexto GPU supera completamente os benefícios do paralelismo massivo. O speedup resultante foi de 0.000638, ou seja, esta versão é aproximadamente 1568 vezes mais lenta que a sequencial para este tamanho de entrada.

- Comparação com a versão CUDA: A versão OpenMP GPU apresentou tempo total de 0.1255s contra 0.1581s da versão CUDA, resultando em um tempo total cerca de 20% menor. No entanto, o tempo de execução dos kernels foi maior no OpenMP GPU (0.0043s) do que no CUDA (0.0014s), o que é esperado: as diretivas de offload introduzem overhead adicional de sincronização e gerenciamento que o CUDA nativo não possui. A diferença no tempo total favorável ao OpenMP GPU está relacionada a diferente estratégia de alocação e transferência de memória gerenciada pelo runtime libgomp em comparação com as chamadas explícitas cudaMalloc e cudaMemcpy do CUDA. Os clusters gerados foram idênticos entre as duas versões GPU.

- Observação sobre a compilação: O GCC 13 no Ubuntu 24.04 só suporta até sm_80 no backend nvptx, então o binário foi compilado para sm_80 mesmo rodando numa RTX 4050 (sm_89), o que funcionou por compatibilidade retroativa. Também foi necessário desabilitar o stack protector e a proteção de fluxo de controle nas flags de offload, pois o PTX não tem suporte a essas proteções do Linux e o JIT falhava sem isso.
