## Resumo

Esta versao tem como objetivo implementar o K-means usando diretivas OpenMP com offload para GPU. Diferente da versao CUDA, em que os kernels sao escritos explicitamente, aqui a paralelizacao deve ser expressa com diretivas OpenMP, transferindo regioes de computacao para o dispositivo acelerador.

O foco desta implementacao e avaliar se uma abordagem baseada em diretivas consegue obter ganho de desempenho com menor complexidade de codigo.

## 1. Objetivo

O objetivo desta etapa e adaptar a versao sequencial para executar a parte mais custosa do K-means na GPU por meio de OpenMP offload.

Essa versao deve permitir analisar:

1. O custo de transferencia entre CPU e GPU.
2. O tempo gasto na atribuicao dos pontos aos clusters.
3. A diferenca de desempenho em relacao a versao sequencial.
4. A diferenca de desempenho em relacao a versao CUDA.

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

Na versao OpenMP GPU, a ideia e manter a estrutura geral do K-means em C e usar diretivas OpenMP para enviar dados ao dispositivo e paralelizar os loops principais.

O fluxo esperado e:

1. Ler o dataset na CPU.
2. Inicializar centroides com a mesma estrategia da versao sequencial.
3. Criar regioes `target data` para mapear dataset, centroides e labels.
4. Usar diretivas OpenMP para paralelizar a atribuicao dos pontos aos clusters na GPU.
5. Atualizar centroides usando reducoes ou somas parciais.
6. Repetir ate convergir ou atingir o limite maximo de iteracoes.
7. Copiar labels finais para a CPU.
8. Salvar o resultado em CSV.

## 4. Organizacao do Codigo

Preencher quando a implementacao OpenMP GPU estiver pronta.

| Arquivo | Funcao |
|---|---|
| `main.c` | Ponto de entrada da versao OpenMP GPU |
| `Makefile` | Compilacao com suporte a OpenMP offload |
| `../utils/dataset_config.h` | Configuracao do dataset, numero de clusters e features |
| `../utils/io_utils.c` | Leitura do CSV e escrita dos resultados |
| `../utils/math_utils.c` | Funcoes auxiliares compartilhadas, se aplicavel |

## 5. Como Compilar e Rodar

Com o Makefile criado, a execucao deve seguir o modelo:

```bash
cd "../PROJETO_FINAL_ALTO_DESEMPENHO/src/openmp_gpu"
make
./kmeans_openmp_gpu
```

Para limpar os arquivos gerados:

```bash
make clean
```

Registrar aqui, se necessario, flags especificas usadas pelo compilador:

```bash
gcc -fopenmp -foffload=___ main.c ../utils/io_utils.c ../utils/math_utils.c -o kmeans_openmp_gpu -lm
```

## 6. Saida Esperada

Preencher esta secao com a saida real do terminal apos a execucao.

```text
Carregou ___ pontos com ___ features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Iniciando K-Means OpenMP GPU
K-means concluido com sucesso em ___ interacoes
Tempo total: ___ segundos
Tempo de offload/transferencia: ___ segundos
Clusters salvo: ../../data/processed/results_openmp_gpu.csv
```

## 7. Resultado Gerado

O arquivo de saida esperado e:

```text
../../data/processed/results_openmp_gpu.csv
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

| Versao | Tempo total (s) | Tempo de offload/transferencia (s) | Iteracoes | Speedup |
|---|---:|---:|---:|---:|
| Sequencial | 0.000080000 | - | 9 | 1.00 |
| OpenMP GPU | _preencher_ | _preencher_ | _preencher_ | _preencher_ |

## 9. Calculo das Metricas

O speedup deve ser calculado por:

```text
speedup = tempo_sequencial / tempo_openmp_gpu
```

A eficiencia pode ser calculada por:

```text
eficiencia = speedup / quantidade_total_de_unidades_de_execucao
```

## 10. Discussao dos Resultados

Preencher apos os testes.

Pontos a discutir:

1. Se os clusters ficaram equivalentes aos da versao sequencial.
2. Impacto da transferencia CPU-GPU.
3. Facilidade de implementacao em comparacao com CUDA.
4. Desempenho obtido em relacao a versao sequencial.
5. Desempenho obtido em relacao a versao CUDA.

## 11. Conclusao

Preencher apos a implementacao e execucao dos testes.

A conclusao deve indicar se o OpenMP GPU foi vantajoso para este problema, quais gargalos apareceram e se a abordagem por diretivas foi suficiente para obter bom desempenho.
