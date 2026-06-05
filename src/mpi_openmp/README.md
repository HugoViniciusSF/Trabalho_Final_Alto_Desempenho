## Resumo

Esta versão tem como objetivo implementar o K-means usando paralelismo híbrido em CPU. O MPI deve ser usado para distribuir os dados entre processos, enquanto o OpenMP deve explorar paralelismo com threads dentro de cada processo.

A ideia é comparar esta abordagem com a versão sequencial, avaliando tempo de execução, speedup, eficiência, escalabilidade e custos de comunicação.

## 1. Objetivo

O objetivo desta etapa é dividir o trabalho do K-means entre vários processos MPI e, dentro de cada processo, paralelizar o cálculo das distâncias com OpenMP.

Essa versão deve responder principalmente:

1. Quanto desempenho é ganho ao aumentar o número de processos MPI?
2. Quanto desempenho é ganho ao aumentar o número de threads OpenMP?
3. Qual o impacto da comunicação entre processos na atualização dos centroides?
4. A versão híbrida produz clusters equivalentes aos da versão sequencial?

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

Preencher quando a implementação MPI + OpenMP estiver pronta.

| Arquivo | Função |
|---|---|
| `main.c` | Ponto de entrada da versão híbrida |
| `Makefile` | Compilação com MPI e OpenMP |
| `../utils/dataset_config.h` | Configuração do dataset, número de clusters e features |
| `../utils/io_utils.c` | Leitura do CSV e escrita dos resultados |
| `../utils/math_utils.c` | Funções auxiliares compartilhadas, se aplicável |

## 5. Como Compilar e Rodar

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

## 6. Saída Esperada

Preencher esta seção com a saída real do terminal após a execução.

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

O arquivo de saída esperado é:

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

## 8. Resultados de Desempenho

Preencher a tabela abaixo após os testes.

| Processos MPI | Threads por processo | Total de threads | Tempo (s) | Iterações | Speedup | Eficiência |
|---:|---:|---:|---:|---:|---:|---:|
| 1 | 1 | 1 | 0.000080000 | 9 | 1.00 | 1.00 |
| _preencher_ | _preencher_ | _preencher_ | _preencher_ | _preencher_ | _preencher_ | _preencher_ |

## 9. Cálculo das Métricas

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

## 10. Discussão dos Resultados

Preencher após os testes.

Pontos a discutir:

1. Impacto do aumento de processos MPI.
2. Impacto do aumento de threads OpenMP.
3. Custo de comunicação na sincronização dos centroides.
4. Balanceamento de carga entre processos.
5. Comparação dos clusters com a versão sequencial.

## 11. Conclusão

Preencher após a implementação e execução dos testes.

A conclusão deve indicar se a abordagem híbrida trouxe ganho de desempenho, em quais configurações o ganho foi melhor e quais fatores limitaram a escalabilidade.
