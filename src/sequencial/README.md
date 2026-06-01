## Resumo

Esta versao implementa o algoritmo K-means de forma sequencial, usando apenas uma CPU e uma unica linha de execucao. Ela serve como baseline do projeto, ou seja, como referencia para comparar as versoes paralelas com MPI, OpenMP, OpenMP GPU e CUDA.

O programa le o dataset tratado dos pinguins, seleciona quatro atributos numericos normalizados, inicializa tres centroides e executa o K-means ate a convergencia ou ate atingir o limite maximo de iteracoes. Ao final, salva em um arquivo CSV o cluster atribuido a cada ponto do dataset.

## 1. Objetivo

O objetivo desta etapa e construir uma versao correta e reprodutivel do K-means antes de paralelizar o algoritmo. Essa versao mede o tempo apenas da etapa principal de agrupamento, isto e, da chamada de `run_kmeans`, sem incluir o tempo de leitura do arquivo CSV e escrita dos resultados.

Essa separacao e importante porque as outras implementacoes devem ser comparadas pelo custo computacional do algoritmo, e nao pelo custo de entrada e saida de arquivos.

## 2. Entrada de Dados

O arquivo de entrada e:

```text
../../data/processed/penguins_clean.csv
```

O caminho acima e relativo a pasta `src/sequencial`, por isso o executavel deve ser rodado a partir dessa pasta.

As colunas usadas pelo K-means estao definidas em `src/utils/dataset_config.h`:

| Atributo | Descricao |
|---|---|
| `bill_length_mm` | Comprimento do bico |
| `bill_depth_mm` | Profundidade do bico |
| `flipper_length_mm` | Comprimento da nadadeira |
| `body_mass_g` | Massa corporal |

Essas colunas ja foram normalizadas no pre-processamento, usando escala Min-Max entre 0 e 1. A normalizacao e necessaria porque o K-means usa distancia euclidiana; sem normalizar, atributos com escala maior, como massa corporal, teriam peso excessivo no calculo das distancias.

## 3. Funcionamento do K-means

O K-means e um algoritmo de agrupamento nao supervisionado. Isso significa que ele nao usa diretamente a coluna `species` para aprender. Em vez disso, ele tenta separar os pontos em grupos com base na similaridade numerica entre as medidas dos pinguins.

Nesta implementacao, o numero de clusters e:

```c
#define NUM_CLUSTERS 3
```

O valor 3 foi escolhido porque o dataset possui tres especies de pinguins. Mesmo assim, os clusters gerados pelo algoritmo nao recebem automaticamente os nomes das especies. Eles sao apenas identificadores numericos: `0`, `1` e `2`.

O fluxo do algoritmo é:

1. Ler os pontos do CSV tratado.
2. Inicializar os centroides escolhendo pontos aleatorios do dataset.
3. Para cada ponto, calcular a distancia euclidiana ate cada centroide.
4. Atribuir o ponto ao cluster do centroide mais proximo.
5. Recalcular cada centroide usando a media dos pontos do cluster.
6. Repetir ate que nenhum ponto mude de cluster ou ate atingir 100 iteracoes.

Para garantir reprodutibilidade, a inicializacao dos centroides usa semente fixa:

```c
srand(42);
```

Assim, execucoes diferentes tendem a gerar os mesmos centroides iniciais e os mesmos agrupamentos, desde que o dataset de entrada nao mude.

## 4. Organizacao do Codigo

| Arquivo | Funcao |
|---|---|
| `main.c` | Controla a execucao: le o dataset, inicializa estruturas, mede tempo, roda o K-means e salva resultados |
| `Makefile` | Compila o programa sequencial |
| `../utils/dataset_config.h` | Define caminho do dataset, quantidade de clusters e features usadas |
| `../utils/io_utils.c` | Implementa leitura do CSV e escrita do resultado |
| `../utils/math_utils.c` | Implementa distancia euclidiana, inicializacao dos centroides e loop do K-means |

## 5. Como Compilar e Rodar

Acesse a pasta da versao sequencial:

```bash
cd "../PROJETO_FINAL_ALTO_DESEMPENHO/src/sequencial"
```

Compile com:

```bash
make
```

Execute com:

```bash
./kmeans_seq
```

Para remover o executavel gerado:

```bash
make clean
```

Tambem e possivel compilar a partir da raiz do projeto:

```bash
make -C src/sequencial
```

Entretanto, a execucao deve ocorrer dentro de `src/sequencial`, porque o caminho do dataset no codigo e relativo a essa pasta.

## 6. Saida Esperada

Em uma execucao local, o programa apresentou a seguinte saida:

```text
Carregou 333 pontos com 4 features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Iniciando K-Means
K-means concluido com sucesso em 9 interacoes
Wall Clock Time: 0.000080000 segundos
Clusters salvo: ../../data/processed/results_sequencial.csv
```

O tempo pode variar entre execucoes e maquinas, mas a ordem de grandeza deve permanecer muito baixa para este dataset, pois ele possui apenas 333 pontos e 4 atributos numericos.

## 7. Resultado Gerado

O arquivo de saida e:

```text
../../data/processed/results_sequencial.csv
```

Ele possui duas colunas:

| Coluna | Significado |
|---|---|
| `point_id` | Indice do ponto no dataset tratado, iniciando em 0 |
| `cluster_id` | Cluster atribuido pelo K-means ao ponto |

Exemplo:

```csv
point_id,cluster_id
0,1
1,0
2,0
3,0
```

Isso significa que o primeiro ponto do dataset foi classificado no cluster 1, enquanto os pontos 1, 2 e 3 foram classificados no cluster 0.

## 8. Distribuicao dos Clusters

Na execucao realizada, os 333 pontos ficaram distribuidos da seguinte forma:

| Cluster | Quantidade de pontos |
|---:|---:|
| 0 | 119 |
| 1 | 124 |
| 2 | 90 |

Comparando os clusters com a coluna `species` do dataset tratado, obteve-se:

| Especie | Cluster 0 | Cluster 1 | Cluster 2 |
|---|---:|---:|---:|
| Adelie | 0 | 116 | 30 |
| Chinstrap | 0 | 8 | 60 |
| Gentoo | 119 | 0 | 0 |

Essa comparacao nao e usada pelo algoritmo durante o treinamento. Ela serve apenas para interpretar o resultado depois da execucao.

## 9. Calculo das Metricas

Valores usados:

| Metrica base | Valor |
|---|---:|
| Pontos processados | 333 |
| Features | 4 |
| Iteracoes | 9 |
| Tempo sequencial | 0.000080000 s |
| Unidades de execucao | 1 |

No sequencial, nao existe tempo paralelo. O tempo medido aqui e usado como base para comparar as outras versoes:

```text
T1 = tempo_sequencial = 0.000080000 s
```

Como esta versao e a referencia, o speedup fica:

```text
speedup = T1 / T1
speedup = 0.000080000 / 0.000080000
speedup = 1.00
```

Como a execucao usa apenas uma unidade de execucao, a eficiencia fica:

```text
eficiencia = 1.00 / 1
eficiencia = 1.00
```

As metricas de escalabilidade forte e fraca ficam para as versoes paralelas, variando processos, threads ou recursos de GPU.

Outras medidas derivadas:

```text
tempo_por_iteracao = tempo_total / iteracoes
tempo_por_iteracao = 0.000080000 / 9
tempo_por_iteracao = 0.000008889 s

tempo_por_ponto = tempo_total / pontos
tempo_por_ponto = 0.000080000 / 333
tempo_por_ponto = 0.000000240 s
```

## 10. Discussao dos Resultados

O cluster 0 agrupou todos os pontos da especie Gentoo presentes no dataset tratado. Isso indica que, com os atributos numericos selecionados, os pinguins Gentoo ficaram bem separados das demais especies no espaco de caracteristicas.

Os clusters 1 e 2 dividiram principalmente as especies Adelie e Chinstrap. O cluster 1 concentrou a maior parte dos Adelie, enquanto o cluster 2 concentrou a maior parte dos Chinstrap. Ainda assim, houve mistura entre essas duas especies, especialmente com 30 pinguins Adelie no cluster 2 e 8 Chinstrap no cluster 1.


## 11. Conclusao

A implementacao sequencial executa corretamente o fluxo basico do K-means: leitura dos dados, inicializacao dos centroides, atribuicao dos pontos aos clusters, atualizacao dos centroides, criterio de parada e escrita dos resultados.

Como baseline, esta versao e fundamental para o restante do projeto. As versoes paralelas devem produzir agrupamentos equivalentes ou muito proximos e reduzir o tempo de execucao quando aplicadas a entradas maiores ou a cargas de trabalho mais pesadas.
