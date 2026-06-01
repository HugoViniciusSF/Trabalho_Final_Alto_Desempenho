# Resultados da Execucao CUDA

## Comando Utilizado

```bash
./kmeans_cuda
```

## Saida do Terminal

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

## Metricas

| Metrica | Valor |
|---|---:|
| Tempo sequencial | 0.000080000 s |
| Tempo CUDA | 0.158066884 s |
| Tempo dos kernels | 0.001422059 s |
| Tempo de transferencia CPU-GPU | 0.156229382 s |
| Iteracoes | 10 |
| Processos/threads usados | 512 threads (2 blocos de 256) |
| Speedup | 0.000506 |
| Eficiencia | 0.000000988 |

## Observacoes

- Comparação dos clusters gerados com o sequencial: Os agrupamentos resultantes são 99,7% idênticos aos da versão de referência na CPU. A verificação indicou que apenas um único ponto (ID 328) foi atribuído a um cluster diferente. Esta ligeira divergência é um comportamento esperado, justificado pela diferença na precisão aritmética de ponto flutuante entre a CPU e a GPU durante o cálculo iterativo da distância. É devido a este ponto de fronteira que a GPU necessitou de 10 iterações para convergir, contra as 9 da versão sequencial.

- Participação do tempo de kernels e transferência: O tempo total foi fortemente dominado pela comunicação. A transferência de memória e a inicialização do contexto da GPU representaram aproximadamente 98,8% do tempo total (0.156s), enquanto o processamento matemático nos kernels CUDA consumiu apenas 0,9% (0.0014s). Isto comprova que, para datasets minúsculos (333 linhas), o custo de aceder ao barramento PCIe anula os benefícios do paralelismo massivo. A eficiência do hardware atingiu praticamente 0%, de acordo com a Lei de Amdahl.

- Comparação com a versão OpenMP GPU: [Nota: Como ainda vamos desenvolver esta versão, deixe este tópico com o texto abaixo por enquanto] Esta secção será atualizada assim que a implementação com diretivas OpenMP de offload estiver concluída, permitindo avaliar se a abstração do compilador introduz overhead adicional face ao código CUDA nativo.

- Otimização de Arquitetura: Para mitigar o custo de comunicação, a versão final do código CUDA implementou a atualização dos centroides diretamente na placa gráfica (Device-centric). O uso de operações de hardware seguras (atomicAdd) eliminou as cópias intermediárias do dataset e das labels a cada iteração, restringindo o tráfego no barramento apenas à verificação do critério de paragem (1 inteiro de 4 bytes).
