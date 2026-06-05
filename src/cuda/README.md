## Resumo

Esta versão tem como objetivo implementar o K-means usando CUDA, explorando o paralelismo explícito da GPU. Diferente da versão sequencial, em que todos os pontos são processados por uma única linha de execução na CPU, a versão CUDA deve distribuir o cálculo de distâncias e a atribuição dos clusters entre várias threads da GPU.

O foco desta implementação é avaliar o ganho de desempenho obtido com kernels CUDA, considerando também o custo de alocação de memória e transferência de dados entre CPU e GPU.

## 1. Objetivo

O objetivo desta etapa é construir uma versão em GPU capaz de executar a etapa principal do K-means em paralelo. A implementação deve usar a versão sequencial como referência de corretude e como baseline para cálculo de speedup.

A medição deve separar, quando possível:

1. Tempo total da execução.
2. Tempo gasto nos kernels CUDA.
3. Tempo gasto em cópias entre CPU e GPU.

Essa separação é importante porque, em datasets pequenos, o custo de transferência pode ser maior do que o ganho obtido pelo paralelismo da GPU.

## 2. Entrada de Dados

O arquivo de entrada esperado é:

```text
../../data/processed/penguins_clean.csv
```

As features usadas devem ser as mesmas da versão sequencial:

| Atributo | Descrição |
|---|---|
| `bill_length_mm` | Comprimento do bico |
| `bill_depth_mm` | Profundidade do bico |
| `flipper_length_mm` | Comprimento da nadadeira |
| `body_mass_g` | Massa corporal |

Manter as mesmas features permite comparar os resultados da versão CUDA com os resultados da versão sequencial.

## 3. Estratégia de Paralelização

Na versão CUDA, a etapa mais natural para paralelização é a atribuição dos pontos aos clusters. Cada thread pode processar um ponto do dataset, calcular sua distância até todos os centroides e escolher o cluster mais próximo.

O fluxo esperado é:

1. Ler o dataset na CPU.
2. Inicializar os centroides na CPU, usando a mesma semente da versão sequencial.
3. Alocar memória na GPU para dataset, centroides, novos centroides, labels e tamanhos dos clusters.
4. Copiar os dados iniciais da CPU para a GPU.
5. Executar kernel para calcular distâncias e atribuir clusters.
6. Atualizar centroides a partir das somas parciais dos pontos.
7. Repetir até convergir ou atingir o limite máximo de iterações.
8. Copiar os labels finais para a CPU.
9. Salvar o resultado em CSV.

## 4. Organização do Código

| Arquivo | Função |
|---|---|
| `main.cu` | Ponto de entrada da versão CUDA |
| `cuda_kmeans.cu` | Kernels CUDA e função principal da execução em GPU |
| `Makefile` | Compilação com `nvcc` |
| `../utils/dataset_config.h` | Configuração do dataset, número de clusters e features |
| `../utils/io_utils.c` | Leitura do CSV e escrita dos resultados |
| `../utils/math_utils.c` | Inicialização dos centroides e funções auxiliares compartilhadas |

## 5. Como Compilar e Rodar

Com o Makefile criado, a execução deve seguir o modelo:

```bash
cd "../PROJETO_FINAL_ALTO_DESEMPENHO/src/cuda"
make
./kmeans_cuda
```

Para limpar os arquivos gerados:

```bash
make clean
```

Exemplo de compilação direta com `nvcc`:

```bash
nvcc -O3 -arch=sm_61 main.cu cuda_kmeans.cu ../utils/io_utils.c ../utils/math_utils.c -o kmeans_cuda
```

## 6. Saída Esperada

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

## 7. Resultado Gerado

O arquivo de saída esperado é:

```text
../../data/processed/results_cuda.csv
```

Ele deve manter o mesmo formato da versão sequencial:

```csv
point_id,cluster_id
0,_
1,_
2,_
```

## 8. Resultados de Desempenho

| Versão | Tempo total (s) | Tempo dos kernels (s) | Tempo de transferência (s) | Iterações | Speedup |
|---|---:|---:|---:|---:|---:|
| Sequencial | 0.000080000 | - | - | 9 | 1.00 |
| CUDA | 0.158066884 | 0.001422059 | 0.156229382 | 10 | 0.000506 |

## 9. Cálculo das Métricas

O speedup deve ser calculado por:

```text
speedup = tempo_sequencial / tempo_cuda
speedup = 0.000080000 / 0.158066884
speedup = 0.000506
```

A eficiência pode ser calculada por:

```text
eficiencia = speedup / quantidade_total_de_unidades_de_execucao
eficiencia = 0.000506 / 512
eficiencia = 0.000000988
```


## 10. Discussão dos Resultados

Os clusters gerados pela versão CUDA ficaram 99,7% iguais aos da versão sequencial. A diferença ocorreu em apenas um ponto, o ID 328, que ficou no cluster 1 na GPU e no cluster 2 no sequencial. Essa diferença pequena é esperada em execuções paralelas com ponto flutuante, pois a ordem das somas pode alterar levemente os centroides.

O tempo total foi maior que o sequencial porque o dataset tem apenas 333 pontos. Nesse tamanho, o custo de inicializar a GPU e transferir dados pesa mais do que o ganho do paralelismo. A transferência CPU-GPU representou aproximadamente 98,8% do tempo total, enquanto os kernels ficaram com cerca de 0,9%.

Comparando com OpenMP GPU, a versão CUDA teve kernels mais rápidos, mas tempo total maior. O OpenMP GPU executou em 0.125462111 s, enquanto CUDA executou em 0.158066884 s. Mesmo assim, CUDA teve menor tempo de kernels: 0.001422059 s contra 0.004286660 s no OpenMP GPU.

## 11. Conclusão

A versão CUDA executa o K-means na GPU e produz resultado quase idêntico ao sequencial. Para o dataset Penguins, ela não trouxe ganho de desempenho por causa do tamanho pequeno da entrada e do alto custo de transferência CPU-GPU.

Essa versão tende a ser mais vantajosa em datasets maiores, nos quais o custo de transferência passa a ser compensado pelo volume de cálculo paralelo nos kernels.
