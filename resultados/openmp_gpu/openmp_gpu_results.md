# Resultados da Execucao OpenMP GPU

## Comando Utilizado

```bash
./kmeans_openmp_gpu
```

## Saida do Terminal

```text
Carregou 333 pontos com 4 features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Iniciando K-Means OpenMP GPU
K-means concluido com sucesso em 10 interacoes
Tempo total: 0.125462111 segundos
Tempo de offload/transferencia: 0.121175451 segundos
Clusters salvo: ../../data/processed/results_openmp_gpu.csv
```

## Metricas

| Metrica | Valor |
|---|---:|
| Tempo sequencial | 0.000080000 s |
| Tempo OpenMP GPU | 0.125462111 s |
| Tempo de offload/transferencia | 0.121175451 s |
| Tempo de kernels (calculado) | 0.004286660 s |
| Iteracoes | 10 |
| Processos/threads usados | Automatico pelo runtime OpenMP (nvptx-none, sm_80) |
| Speedup | 0.000638 |

## Observacoes

- Comparacao dos clusters gerados com `data/processed/results_sequencial.csv`: Os agrupamentos resultantes sao identicos aos da versao CUDA e diferem do sequencial em exatamente um ponto (ID 328, atribuido ao cluster 1 em vez do cluster 2). Esta divergencia minima e esperada e decorre da diferenca na ordem das operacoes atomicas de ponto flutuante em execucao paralela, alterando levemente o calculo dos centroides a cada iteracao. E por este ponto de fronteira que ambas as versoes GPU precisaram de 10 iteracoes para convergir, contra as 9 da versao sequencial.

- Participacao do tempo de offload e transferencia: O tempo total foi fortemente dominado pela comunicacao CPU-GPU. A transferencia de dados e a inicializacao do contexto OpenMP na GPU representaram aproximadamente 96,6% do tempo total (0.1212s), enquanto a execucao dos kernels de K-means consumiu apenas 3,4% (0.0043s). Isso confirma, em linha com o que foi observado na versao CUDA, que para datasets minusculos como o Penguins (333 pontos), o custo de acesso ao barramento PCIe e de inicializacao do contexto GPU supera completamente os beneficios do paralelismo massivo. O speedup resultante foi de 0.000638, ou seja, esta versao e aproximadamente 1568 vezes mais lenta que a sequencial para este tamanho de entrada.

- Comparacao com a versao CUDA: A versao OpenMP GPU apresentou tempo total de 0.1255s contra 0.1581s da versao CUDA, resultando em um tempo total cerca de 20% menor. No entanto, o tempo de execucao dos kernels foi maior no OpenMP GPU (0.0043s) do que no CUDA (0.0014s), o que e esperado: as diretivas de offload introduzem overhead adicional de sincronizacao e gerenciamento que o CUDA nativo nao possui. A diferenca no tempo total favoravel ao OpenMP GPU esta relacionada a diferente estrategia de alocacao e transferencia de memoria gerenciada pelo runtime libgomp em comparacao com as chamadas explicitas cudaMalloc e cudaMemcpy do CUDA. Os clusters gerados foram identicos entre as duas versoes GPU.

- Observacao sobre a compilacao: O GCC 13 no Ubuntu 24.04 so suporta ate sm_80 no backend nvptx, entao o binario foi compilado para sm_80 mesmo rodando numa RTX 4050 (sm_89), o que funcionou por compatibilidade retroativa. Tambem foi necessario desabilitar o stack protector e a protecao de fluxo de controle nas flags de offload, pois o PTX nao tem suporte a essas protecoes do Linux e o JIT falhava sem isso.
