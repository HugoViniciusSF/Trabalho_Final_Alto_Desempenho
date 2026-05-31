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

Preencher quando a implementacao CUDA estiver pronta.

| Arquivo | Funcao |
|---|---|
| `main.cu` | Ponto de entrada da versao CUDA |
| `Makefile` | Compilacao com `nvcc` |
| `../utils/dataset_config.h` | Configuracao do dataset, numero de clusters e features |
| `../utils/io_utils.c` | Leitura do CSV e escrita dos resultados |
| `../utils/math_utils.c` | Funcoes auxiliares compartilhadas, se aplicavel |

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

Caso a compilacao seja feita diretamente com `nvcc`, registrar aqui o comando final usado:

```bash
nvcc main.cu ../utils/io_utils.c -o kmeans_cuda -lm
```

## 6. Saida Esperada

Preencher esta secao com a saida real do terminal apos a execucao.

```text
Carregou ___ pontos com ___ features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Iniciando K-Means CUDA
K-means concluido com sucesso em ___ interacoes
Tempo total: ___ segundos
Tempo dos kernels: ___ segundos
Tempo de transferencia CPU-GPU: ___ segundos
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

Preencher a tabela abaixo apos a execucao.

| Versao | Tempo total (s) | Tempo dos kernels (s) | Tempo de transferencia (s) | Iteracoes |
|---|---:|---:|---:|---:|
| Sequencial | 0.000080000 | - | - | 9 |
| CUDA | _preencher_ | _preencher_ | _preencher_ | _preencher_ |

## 9. Calculo das Metricas

O speedup deve ser calculado por:

```text
speedup = tempo_sequencial / tempo_cuda
```

A eficiencia pode ser calculada por:

```text
eficiencia = speedup / quantidade_total_de_unidades_de_execucao
```


## 10. Discussao dos Resultados

Preencher apos os testes.

Pontos a discutir:

1. Se a versao CUDA produziu clusters equivalentes aos da versao sequencial.
2. Se o tempo total melhorou ou piorou em relacao ao sequencial.
3. Quanto da execucao foi gasto em kernels.
4. Quanto da execucao foi gasto em transferencia CPU-GPU.
5. Se o tamanho do dataset foi suficiente para justificar o uso da GPU.

## 11. Conclusao

Preencher apos a implementacao e execucao dos testes.

A conclusao deve indicar se a versao CUDA trouxe ganho real de desempenho, quais foram os principais gargalos e em quais condicoes a abordagem em GPU tende a ser mais vantajosa.
