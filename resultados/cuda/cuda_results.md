# Resultados da Execucao CUDA

## Saida do Terminal

```
Carregou 333 pontos com 4 features.
Iniciando K-Means na GPU (CUDA com AtomicAdd)
K-means (CUDA) concluido com sucesso em 10 interacoes

=== RESULTADOS DE DESEMPENHO CUDA ===
Tempo total (s): 0.160496702 segundos
Tempo dos kernels (s): 0.001312263 segundos
Tempo de transferencia (s): 0.158815456 segundos
Clusters salvo: ../../data/processed/results_cuda.csv
```

## Observacao

A execucao utilizou o dataset tratado data/processed/penguins_clean.csv, com 333 pontos e 4 atributos numericos. O algoritmo foi executado em paralelo na GPU (NVIDIA GTX 1050 Ti), utilizando a tecnica de AtomicAdd para a atualizacao dos centroides diretamente na memoria do dispositivo (Device).

O numero de iteracoes (10) divergiu em uma unidade em relacao ao sequencial (9) devido a diferencas na precisao aritmetica de ponto flutuante (floating point arithmetic) entre o hardware da CPU e os nucleos CUDA, resultando na divergencia de classificacao de um unico ponto (ID 328) na fronteira de clusterizacao. O arquivo gerado com os clusters foi salvo em `data/processed/results_cuda.csv`.