## Resumo

Esta versao tem como objetivo implementar o K-means usando paralelismo hibrido em CPU. O MPI deve ser usado para distribuir os dados entre processos, enquanto o OpenMP deve explorar paralelismo com threads dentro de cada processo.

A ideia e comparar esta abordagem com a versao sequencial, avaliando tempo de execucao, speedup, eficiencia, escalabilidade e custos de comunicacao.

## 1. Objetivo

O objetivo desta etapa e dividir o trabalho do K-means entre varios processos MPI e, dentro de cada processo, paralelizar o calculo das distancias com OpenMP.

Essa versao deve responder principalmente:

1. Quanto desempenho e ganho ao aumentar o numero de processos MPI?
2. Quanto desempenho e ganho ao aumentar o numero de threads OpenMP?
3. Qual o impacto da comunicacao entre processos na atualizacao dos centroides?
4. A versao hibrida produz clusters equivalentes aos da versao sequencial?

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

## 3. Estrategia de Paralelizacao

Nesta versao, o dataset deve ser dividido entre processos MPI. Cada processo fica responsavel por uma parte dos pontos. Dentro de cada processo, OpenMP pode paralelizar o loop que calcula as distancias entre pontos locais e centroides.

O fluxo esperado e:

1. Processo principal le o CSV tratado.
2. Os pontos sao distribuidos entre os processos MPI.
3. Todos os processos recebem os centroides atuais.
4. Cada processo calcula localmente o cluster mais proximo para seus pontos.
5. Cada processo calcula somas parciais e quantidades locais por cluster.
6. MPI combina as somas parciais para formar os novos centroides globais.
7. O processo se repete ate convergir ou atingir o limite maximo de iteracoes.
8. Os labels finais sao reunidos e salvos em CSV.

## 4. Organizacao do Codigo

Preencher quando a implementacao MPI + OpenMP estiver pronta.

| Arquivo | Funcao |
|---|---|
| `main.c` | Ponto de entrada da versao hibrida |
| `Makefile` | Compilacao com MPI e OpenMP |
| `../utils/dataset_config.h` | Configuracao do dataset, numero de clusters e features |
| `../utils/io_utils.c` | Leitura do CSV e escrita dos resultados |
| `../utils/math_utils.c` | Funcoes auxiliares compartilhadas, se aplicavel |

## 5. Como Compilar e Rodar

Com o Makefile criado, a execucao deve seguir o modelo:

```bash
cd "../PROJETO_FINAL_ALTO_DESEMPENHO/src/mpi_openmp"
make
```

Exemplo de execucao com 2 processos MPI e 4 threads OpenMP por processo:

```bash
OMP_NUM_THREADS=4 mpirun -np 2 ./kmeans_mpi_openmp
```

Para limpar os arquivos gerados:

```bash
make clean
```

## 6. Saida Esperada

Preencher esta secao com a saida real do terminal apos a execucao.

```text
Carregou ___ pontos com ___ features.
Processos MPI: ___
Threads OpenMP por processo: ___
Iniciando K-Means MPI + OpenMP
K-means concluido com sucesso em ___ interacoes
Wall Clock Time: ___ segundos
Clusters salvo: ../../data/processed/results_mpi_openmp.csv
```

## 7. Resultado Gerado

O arquivo de saida esperado e:

```text
../../data/processed/results_mpi_openmp.csv
```

Ele deve manter o mesmo formato da versao sequencial:

```csv
point_id,cluster_id
0,_
1,_
2,_
```

## 8. Resultados de Desempenho

Preencher a tabela abaixo apos os testes.

| Processos MPI | Threads por processo | Total de threads | Tempo (s) | Iteracoes | Speedup | Eficiencia |
|---:|---:|---:|---:|---:|---:|---:|
| 1 | 1 | 1 | 0.000080000 | 9 | 1.00 | 1.00 |
| _preencher_ | _preencher_ | _preencher_ | _preencher_ | _preencher_ | _preencher_ | _preencher_ |

## 9. Calculo das Metricas

O speedup deve ser calculado por:

```text
speedup = tempo_sequencial / tempo_paralelo
```

A eficiencia pode ser calculada por:

```text
eficiencia = speedup / quantidade_total_de_unidades_de_execucao
```

Na versao hibrida, a quantidade total de unidades de execucao pode ser considerada como:

```text
processos_mpi * threads_openmp_por_processo
```

## 10. Discussao dos Resultados

Preencher apos os testes.

Pontos a discutir:

1. Impacto do aumento de processos MPI.
2. Impacto do aumento de threads OpenMP.
3. Custo de comunicacao na sincronizacao dos centroides.
4. Balanceamento de carga entre processos.
5. Comparacao dos clusters com a versao sequencial.

## 11. Conclusao

Preencher apos a implementacao e execucao dos testes.

A conclusao deve indicar se a abordagem hibrida trouxe ganho de desempenho, em quais configuracoes o ganho foi melhor e quais fatores limitaram a escalabilidade.
