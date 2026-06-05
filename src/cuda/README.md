## Resumo

Esta versao tem como objetivo implementar o K-means usando CUDA, explorando o paralelismo explicito da GPU. Diferente da versao sequencial, em que todos os pontos sao processados por uma unica linha de execucao na CPU, a versao CUDA deve distribuir o calculo de distancias e a atribuicao dos clusters entre varias threads da GPU.

O foco desta implementacao e avaliar o ganho de desempenho obtido com kernels CUDA, considerando tambem o custo de alocacao de memoria e transferencia de dados entre CPU e GPU.

## 1. Objetivo

O objetivo desta etapa e construir uma versao em GPU capaz de executar a etapa principal do K-means em paralelo. A implementacao deve usar a versao sequencial como referencia de corretude e como baseline para calculo de speedup.

A medicao deve separar, quando possivel:

1. Tempo total da execucao.
2. Tempo gasto nos kernels CUDA.
3. Tempo gasto em copias entre CPU e GPU.

Essa separacao e importante porque, em datasets pequenos, o custo de transferencia pode ser maior do que o ganho obtido pelo paralelismo da GPU.

## 2. Entrada de Dados

O arquivo de entrada esperado e:

```text
../../data/processed/penguins_clean.csv
```

As features usadas devem ser as mesmas da versao sequencial:

| Atributo | Descricao |
|---|---|
| `bill_length_mm` | Comprimento do bico |
| `bill_depth_mm` | Profundidade do bico |
| `flipper_length_mm` | Comprimento da nadadeira |
| `body_mass_g` | Massa corporal |

Manter as mesmas features permite comparar os resultados da versao CUDA com os resultados da versao sequencial.

## 3. Estrategia de Paralelizacao

Na versao CUDA, a etapa mais natural para paralelizacao e a atribuicao dos pontos aos clusters. Cada thread pode processar um ponto do dataset, calcular sua distancia ate todos os centroides e escolher o cluster mais proximo.

O fluxo esperado e:

1. Ler o dataset na CPU.
2. Inicializar os centroides na CPU, usando a mesma semente da versao sequencial.
3. Alocar memoria na GPU para dataset, centroides, novos centroides, labels e tamanhos dos clusters.
4. Copiar os dados iniciais da CPU para a GPU.
5. Executar kernel para calcular distancias e atribuir clusters.
6. Atualizar centroides a partir das somas parciais dos pontos.
7. Repetir ate convergir ou atingir o limite maximo de iteracoes.
8. Copiar os labels finais para a CPU.
9. Salvar o resultado em CSV.

## 4. Organizacao do Codigo

| Arquivo | Funcao |
|---|---|
| `main.cu` | Ponto de entrada da versao CUDA |
| `cuda_kmeans.cu` | Kernels CUDA e funcao principal da execucao em GPU |
| `Makefile` | Compilacao com `nvcc` |
| `../utils/dataset_config.h` | Configuracao do dataset, numero de clusters e features |
| `../utils/io_utils.c` | Leitura do CSV e escrita dos resultados |
| `../utils/math_utils.c` | Inicializacao dos centroides e funcoes auxiliares compartilhadas |

## 5. Como Compilar e Rodar

Com o Makefile criado, a execucao deve seguir o modelo:

```bash
cd "../PROJETO_FINAL_ALTO_DESEMPENHO/src/cuda"
make
./kmeans_cuda
```

Para limpar os arquivos gerados:

```bash
make clean
```

Exemplo de compilacao direta com `nvcc`:

```bash
nvcc -O3 -arch=sm_61 main.cu cuda_kmeans.cu ../utils/io_utils.c ../utils/math_utils.c -o kmeans_cuda
```

## 6. Saida Esperada

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

O arquivo de saida esperado e:

```text
../../data/processed/results_cuda.csv
```

Ele deve manter o mesmo formato da versao sequencial:

```csv
point_id,cluster_id
0,_
1,_
2,_
```

## 8. Resultados de Desempenho

| Versao | Tempo total (s) | Tempo dos kernels (s) | Tempo de transferencia (s) | Iteracoes | Speedup |
|---|---:|---:|---:|---:|---:|
| Sequencial | 0.000080000 | - | - | 9 | 1.00 |
| CUDA | 0.158066884 | 0.001422059 | 0.156229382 | 10 | 0.000506 |

## 9. Calculo das Metricas

O speedup deve ser calculado por:

```text
speedup = tempo_sequencial / tempo_cuda
speedup = 0.000080000 / 0.158066884
speedup = 0.000506
```

A eficiencia pode ser calculada por:

```text
eficiencia = speedup / quantidade_total_de_unidades_de_execucao
eficiencia = 0.000506 / 512
eficiencia = 0.000000988
```


## 10. Discussao dos Resultados

Os clusters gerados pela versao CUDA ficaram 99,7% iguais aos da versao sequencial. A diferenca ocorreu em apenas um ponto, o ID 328, que ficou no cluster 1 na GPU e no cluster 2 no sequencial. Essa diferenca pequena e esperada em execucoes paralelas com ponto flutuante, pois a ordem das somas pode alterar levemente os centroides.

O tempo total foi maior que o sequencial porque o dataset tem apenas 333 pontos. Nesse tamanho, o custo de inicializar a GPU e transferir dados pesa mais do que o ganho do paralelismo. A transferencia CPU-GPU representou aproximadamente 98,8% do tempo total, enquanto os kernels ficaram com cerca de 0,9%.

Comparando com OpenMP GPU, a versao CUDA teve kernels mais rapidos, mas tempo total maior. O OpenMP GPU executou em 0.125462111 s, enquanto CUDA executou em 0.158066884 s. Mesmo assim, CUDA teve menor tempo de kernels: 0.001422059 s contra 0.004286660 s no OpenMP GPU.

## 11. Conclusao

A versao CUDA executa o K-means na GPU e produz resultado quase identico ao sequencial. Para o dataset Penguins, ela nao trouxe ganho de desempenho por causa do tamanho pequeno da entrada e do alto custo de transferencia CPU-GPU.

Essa versao tende a ser mais vantajosa em datasets maiores, nos quais o custo de transferencia passa a ser compensado pelo volume de calculo paralelo nos kernels.
