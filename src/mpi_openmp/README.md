## Resumo

Esta versão tem como objetivo implementar o K-means usando paralelismo híbrido em CPU. O MPI deve ser usado para distribuir os dados entre processos, enquanto o OpenMP deve explorar paralelismo com threads dentro de cada processo.

A ideia é comparar esta abordagem com a versão sequencial, avaliando tempo de execução, speedup, eficiência, escalabilidade e custos de comunicação.

## 1. Objetivo

O objetivo desta etapa é dividir o trabalho do K-means entre vários processos MPI e, dentro de cada processo, paralelizar o cálculo das distâncias com OpenMP.

Essa versão deve responder principalmente:

1. Quanto desempenho é ganho ao aumentar o número de processos MPI?
2. Quanto desempenho é ganho ao aumentar o número de threads OpenMP?
3. Qual o impacto da comunicação entre processos na atualização dos centroides?
4. Qual é a concordância dos clusters em relação à versão sequencial?

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

## 3. Estratégia de Paralelização

Nesta versão, o dataset deve ser dividido entre processos MPI. Cada processo fica responsável por uma parte dos pontos. Dentro de cada processo, OpenMP pode paralelizar o loop que calcula as distâncias entre pontos locais e centroides.

O fluxo esperado é:

1. Processo principal lê o CSV tratado.
2. Os pontos são distribuídos entre os processos MPI.
3. Todos os processos recebem os centroides atuais.
4. Cada processo calcula localmente o cluster mais próximo para seus pontos.
5. Cada processo calcula somas parciais e quantidades locais por cluster.
6. MPI combina as somas parciais para formar os novos centroides globais.
7. O processo se repete até convergir ou atingir o limite máximo de iterações.
8. Os labels finais são reunidos e salvos em CSV.

## 4. Organização do Código

| Arquivo | Função |
|---|---|
| `main.c` | Ponto de entrada da versão híbrida. Inicializa MPI, carrega o dataset no processo 0, distribui os dados, mede o tempo e salva os clusters finais. |
| `Makefile` | Compilação com `mpicc`, `-fopenmp` e `-DUSE_MPI`. |
| `../utils/dataset_config.h` | Configuração do dataset, número de clusters e features |
| `../utils/io_utils.c` | Leitura do CSV e escrita dos resultados |
| `../utils/math_utils.c` | Distância euclidiana, inicialização dos centroides e função `run_kmeans_mpi_openmp`. |

## 5. Detalhes da implementação

A implementação atual segue a seguinte organização:

1. O processo 0 lê `data/processed/penguins_clean.csv`.
2. O número de pontos é enviado para todos os processos com `MPI_Bcast`.
3. A divisão dos pontos é calculada com `sendcounts` e `displs`.
4. Os dados são distribuídos com `MPI_Scatterv`.
5. Os centroides iniciais são definidos no processo 0 e enviados aos demais processos com `MPI_Bcast`.
6. Cada processo executa o K-means sobre seus pontos locais.
7. O loop de atribuição de clusters é paralelizado com OpenMP.
8. As somas parciais dos centroides e os tamanhos dos clusters são combinados com `MPI_Allreduce`.
9. Os labels finais são reunidos no processo 0 com `MPI_Gatherv`.
10. O resultado é salvo em `data/processed/results_mpi_openmp.csv`.

## 6. Como Compilar e Rodar

Com o Makefile criado, a execução deve seguir o modelo:

```bash
cd "../PROJETO_FINAL_ALTO_DESEMPENHO/src/mpi_openmp"
make
```

Exemplo de execução com 2 processos MPI e 4 threads OpenMP por processo:

```bash
OMP_NUM_THREADS=4 mpirun -np 2 ./kmeans_mpi_openmp
```

Para limpar os arquivos gerados:

```bash
make clean
```

## 7. Saída do Terminal

```text
Carregou 333 pontos com 4 features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Processos MPI: 1, 2 ou 4
Threads OpenMP por processo: 1, 2 ou 4
Iniciando K-Means MPI + OpenMP
K-means concluido com sucesso em 9 interacoes
Wall Clock Time: valores registrados na tabela de desempenho
Clusters salvo: ../../data/processed/results_mpi_openmp.csv
```

## 8. Resultado Gerado

O arquivo de saída gerado é:

```text
../../data/processed/results_mpi_openmp.csv
```

Ele deve manter o mesmo formato da versão sequencial:

```csv
point_id,cluster_id
0,_
1,_
2,_
```

O CSV possui 333 pontos, mesma quantidade da versão sequencial. A comparação com os demais resultados registrados indicou 99,70% de concordância dos clusters, com divergência apenas no ponto 328.

## 9. Resultados de Desempenho

Tempo sequencial de referência: `0.000080000 s`.

| Processos MPI | Threads por processo | Total de unidades | Tempo (s) | Iterações | Speedup | Eficiência |
|---:|---:|---:|---:|---:|---:|---:|
| 1 | 1 | 1 | 0.000048000 | 9 | 1.666667 | 1.666667 |
| 1 | 2 | 2 | 0.000184000 | 9 | 0.434783 | 0.217391 |
| 1 | 4 | 4 | 0.000313000 | 9 | 0.255591 | 0.063898 |
| 2 | 1 | 2 | 0.000516000 | 9 | 0.155039 | 0.077519 |
| 2 | 2 | 4 | 0.000924000 | 9 | 0.086580 | 0.021645 |
| 2 | 4 | 8 | 0.001251000 | 9 | 0.063949 | 0.007994 |
| 4 | 1 | 4 | 0.000740000 | 9 | 0.108108 | 0.027027 |
| 4 | 2 | 8 | 0.001146000 | 9 | 0.069808 | 0.008726 |

## 10. Cálculo das Métricas

O speedup deve ser calculado por:

```text
speedup = tempo_sequencial / tempo_paralelo
```

A eficiência pode ser calculada por:

```text
eficiencia = speedup / quantidade_total_de_unidades_de_execucao
```

Na versão híbrida, a quantidade total de unidades de execução pode ser considerada como:

```text
processos_mpi * threads_openmp_por_processo
```

## 11. Discussão dos Resultados

A implementação híbrida cobre a divisão dos dados entre processos, a paralelização local com OpenMP e a sincronização global dos centroides. A melhor medição foi obtida com 1 processo MPI e 1 thread OpenMP, com `0.000048000 s`.

O aumento do número de threads e processos não melhorou o desempenho neste dataset. Como a entrada tratada possui apenas 333 pontos, o trabalho computacional é pequeno e os tempos registrados indicam overhead de paralelização relevante.

As execuções com 2 e 4 processos foram mais lentas que a execução com 1 processo. Isso indica que, para este tamanho de entrada, o custo de comunicação e sincronização dos centroides superou o benefício da divisão do trabalho.

O balanceamento de carga não foi medido separadamente. Pelo código, os pontos são distribuídos de forma quase uniforme entre os processos.

Os clusters gerados tiveram alta concordância com a versão sequencial e também com as versões CUDA e OpenMP GPU. A única divergência observada ocorreu no ponto 328.

## 12. Conclusão

A versão MPI + OpenMP está implementada e gera clusters com 99,70% de concordância em relação às demais versões registradas. Para o Penguins Dataset, as configurações com mais processos e threads foram mais lentas que a configuração com 1 processo e 1 thread.
