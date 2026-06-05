## Resumo

Esta versão implementa o algoritmo K-means de forma sequencial, usando apenas uma CPU e uma única linha de execução. Ela serve como baseline do projeto, ou seja, como referência para comparar as versões paralelas com MPI, OpenMP, OpenMP GPU e CUDA.

O programa lê o dataset tratado dos pinguins, seleciona quatro atributos numéricos normalizados, inicializa três centroides e executa o K-means até a convergência ou até atingir o limite máximo de iterações. Ao final, salva em um arquivo CSV o cluster atribuído a cada ponto do dataset.

## 1. Objetivo

O objetivo desta etapa é construir uma versão correta e reprodutível do K-means antes de paralelizar o algoritmo. Essa versão mede o tempo apenas da etapa principal de agrupamento, isto é, da chamada de `run_kmeans`, sem incluir o tempo de leitura do arquivo CSV e escrita dos resultados.

Essa separação é importante porque as outras implementações devem ser comparadas pelo custo computacional do algoritmo, e não pelo custo de entrada e saída de arquivos.

## 2. Entrada de Dados

O arquivo de entrada é:

```text
../../data/processed/penguins_clean.csv
```

O caminho acima é relativo à pasta `src/sequencial`, por isso o executável deve ser rodado a partir dessa pasta.

As colunas usadas pelo K-means estão definidas em `src/utils/dataset_config.h`:

| Atributo | Descrição |
|---|---|
| `bill_length_mm` | Comprimento do bico |
| `bill_depth_mm` | Profundidade do bico |
| `flipper_length_mm` | Comprimento da nadadeira |
| `body_mass_g` | Massa corporal |

Essas colunas já foram normalizadas no pré-processamento, usando escala Min-Max entre 0 e 1. A normalização é necessária porque o K-means usa distância euclidiana; sem normalizar, atributos com escala maior, como massa corporal, teriam peso excessivo no cálculo das distâncias.

## 3. Funcionamento do K-means

O K-means é um algoritmo de agrupamento não supervisionado. Isso significa que ele não usa diretamente a coluna `species` para aprender. Em vez disso, ele tenta separar os pontos em grupos com base na similaridade numérica entre as medidas dos pinguins.

Nesta implementação, o número de clusters é:

```c
#define NUM_CLUSTERS 3
```

O valor 3 foi escolhido porque o dataset possui três espécies de pinguins. Mesmo assim, os clusters gerados pelo algoritmo não recebem automaticamente os nomes das espécies. Eles são apenas identificadores numéricos: `0`, `1` e `2`.

O fluxo do algoritmo é:

1. Ler os pontos do CSV tratado.
2. Inicializar os centroides escolhendo pontos aleatórios do dataset.
3. Para cada ponto, calcular a distância euclidiana até cada centroide.
4. Atribuir o ponto ao cluster do centroide mais próximo.
5. Recalcular cada centroide usando a média dos pontos do cluster.
6. Repetir até que nenhum ponto mude de cluster ou até atingir 100 iterações.

Para garantir reprodutibilidade, a inicialização dos centroides usa semente fixa:

```c
srand(42);
```

Assim, execuções diferentes tendem a gerar os mesmos centroides iniciais e os mesmos agrupamentos, desde que o dataset de entrada não mude.

## 4. Organização do Código

| Arquivo | Função |
|---|---|
| `main.c` | Controla a execução: lê o dataset, inicializa estruturas, mede tempo, roda o K-means e salva resultados |
| `Makefile` | Compila o programa sequencial |
| `../utils/dataset_config.h` | Define caminho do dataset, quantidade de clusters e features usadas |
| `../utils/io_utils.c` | Implementa leitura do CSV e escrita do resultado |
| `../utils/math_utils.c` | Implementa distância euclidiana, inicialização dos centroides e loop do K-means |

## 5. Como Compilar e Rodar

Acesse a pasta da versão sequencial:

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

Para remover o executável gerado:

```bash
make clean
```

Também é possível compilar a partir da raiz do projeto:

```bash
make -C src/sequencial
```

Entretanto, a execução deve ocorrer dentro de `src/sequencial`, porque o caminho do dataset no código é relativo a essa pasta.

## 6. Saída Esperada

Em uma execução local, o programa apresentou a seguinte saída:

```text
Carregou 333 pontos com 4 features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Iniciando K-Means
K-means concluido com sucesso em 9 interacoes
Wall Clock Time: 0.000080000 segundos
Clusters salvo: ../../data/processed/results_sequencial.csv
```

O tempo pode variar entre execuções e máquinas, mas a ordem de grandeza deve permanecer muito baixa para este dataset, pois ele possui apenas 333 pontos e 4 atributos numéricos.

## 7. Resultado Gerado

O arquivo de saída é:

```text
../../data/processed/results_sequencial.csv
```

Ele possui duas colunas:

| Coluna | Significado |
|---|---|
| `point_id` | Índice do ponto no dataset tratado, iniciando em 0 |
| `cluster_id` | Cluster atribuído pelo K-means ao ponto |

Exemplo:

```csv
point_id,cluster_id
0,1
1,0
2,0
3,0
```

Isso significa que o primeiro ponto do dataset foi classificado no cluster 1, enquanto os pontos 1, 2 e 3 foram classificados no cluster 0.

## 8. Distribuição dos Clusters

Na execução realizada, os 333 pontos ficaram distribuídos da seguinte forma:

| Cluster | Quantidade de pontos |
|---:|---:|
| 0 | 119 |
| 1 | 124 |
| 2 | 90 |

Comparando os clusters com a coluna `species` do dataset tratado, obteve-se:

| Espécie | Cluster 0 | Cluster 1 | Cluster 2 |
|---|---:|---:|---:|
| Adelie | 0 | 116 | 30 |
| Chinstrap | 0 | 8 | 60 |
| Gentoo | 119 | 0 | 0 |

Essa comparação não é usada pelo algoritmo durante o treinamento. Ela serve apenas para interpretar o resultado depois da execução.

## 9. Cálculo das Métricas

Valores usados:

| Métrica base | Valor |
|---|---:|
| Pontos processados | 333 |
| Features | 4 |
| Iterações | 9 |
| Tempo sequencial | 0.000080000 s |
| Unidades de execução | 1 |

No sequencial, não existe tempo paralelo. O tempo medido aqui é usado como base para comparar as outras versões:

```text
T1 = tempo_sequencial = 0.000080000 s
```

Como esta versão é a referência, o speedup fica:

```text
speedup = T1 / T1
speedup = 0.000080000 / 0.000080000
speedup = 1.00
```

Como a execução usa apenas uma unidade de execução, a eficiência fica:

```text
eficiencia = 1.00 / 1
eficiencia = 1.00
```

As métricas de escalabilidade forte e fraca ficam para as versões paralelas, variando processos, threads ou recursos de GPU.

Outras medidas derivadas:

```text
tempo_por_iteracao = tempo_total / iteracoes
tempo_por_iteracao = 0.000080000 / 9
tempo_por_iteracao = 0.000008889 s

tempo_por_ponto = tempo_total / pontos
tempo_por_ponto = 0.000080000 / 333
tempo_por_ponto = 0.000000240 s
```

## 10. Discussão dos Resultados

O cluster 0 agrupou todos os pontos da espécie Gentoo presentes no dataset tratado. Isso indica que, com os atributos numéricos selecionados, os pinguins Gentoo ficaram bem separados das demais espécies no espaço de características.

Os clusters 1 e 2 dividiram principalmente as espécies Adelie e Chinstrap. O cluster 1 concentrou a maior parte dos Adelie, enquanto o cluster 2 concentrou a maior parte dos Chinstrap. Ainda assim, houve mistura entre essas duas espécies, especialmente com 30 pinguins Adelie no cluster 2 e 8 Chinstrap no cluster 1.


## 11. Conclusão

A implementação sequencial executa corretamente o fluxo básico do K-means: leitura dos dados, inicialização dos centroides, atribuição dos pontos aos clusters, atualização dos centroides, critério de parada e escrita dos resultados.

Como baseline, esta versão é fundamental para o restante do projeto. As versões paralelas devem produzir agrupamentos equivalentes ou muito próximos e reduzir o tempo de execução quando aplicadas a entradas maiores ou a cargas de trabalho mais pesadas.
