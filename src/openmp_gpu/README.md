## Resumo

Esta versão implementa o K-means usando diretivas OpenMP com offload para GPU. Diferente da versão CUDA, em que os kernels são escritos explicitamente, aqui a paralelização é expressa com diretivas OpenMP, transferindo regiões de computação para o dispositivo acelerador via backend nvptx do GCC.

O foco desta implementação é avaliar se uma abordagem baseada em diretivas consegue obter ganho de desempenho com menor complexidade de código, e comparar o desempenho com a versão CUDA nativa.

## 1. Objetivo

O objetivo desta etapa é adaptar a versão sequencial para executar a parte mais custosa do K-means na GPU por meio de OpenMP offload.

Essa versão permite analisar:

1. O custo de transferência entre CPU e GPU.
2. O tempo gasto na atribuição dos pontos aos clusters.
3. A diferença de desempenho em relação à versão sequencial.
4. A diferença de desempenho em relação à versão CUDA.

## 2. Entrada de Dados

O arquivo de entrada esperado é:

```text
../../data/processed/penguins_clean.csv
```

As features usadas são as mesmas da versão sequencial:

| Atributo | Descrição |
|---|---|
| `bill_length_mm` | Comprimento do bico |
| `bill_depth_mm` | Profundidade do bico |
| `flipper_length_mm` | Comprimento da nadadeira |
| `body_mass_g` | Massa corporal |

## 3. Estratégia de Paralelização

A versão OpenMP GPU mantém a estrutura geral do K-means em C e usa diretivas OpenMP para enviar dados ao dispositivo e paralelizar os loops principais.

O fluxo é:

1. Ler o dataset na CPU.
2. Inicializar centróides com a mesma estratégia da versão sequencial (`srand(42)`).
3. Criar regiões `target enter data` para mapear dataset, centróides e labels para a GPU.
4. A cada iteração, paralelizar com `target teams distribute parallel for`:
   - Zerar acumuladores (`cluster_sizes` e `new_centroids`) na GPU.
   - Atribuir cada ponto ao cluster mais próximo (com `atomic` para acumular somas).
   - Atualizar centróides dividindo as somas pelos tamanhos de cada cluster.
5. Detectar convergência com `reduction(max:changed)`.
6. Copiar labels e centróides finais para a CPU com `target exit data`.
7. Salvar resultado em CSV.

## 4. Organização do Código

| Arquivo | Função |
|---|---|
| `main.c` | Ponto de entrada da versão OpenMP GPU |
| `Makefile` | Compilação com suporte a OpenMP offload (nvptx-none, sm_80) |
| `../utils/dataset_config.h` | Configuração do dataset, número de clusters e features |
| `../utils/io_utils.c` | Leitura do CSV e escrita dos resultados |
| `../utils/math_utils.c` | Funções auxiliares compartilhadas |

## 5. Como Compilar e Rodar

### Pré-requisitos (Ubuntu 24.04)

```bash
sudo apt-get install -y gcc-13-offload-nvptx nvptx-tools libgomp-plugin-nvptx1
```

### Compilação e execução

```bash
cd src/openmp_gpu
make
./kmeans_openmp_gpu
```

Para limpar os arquivos gerados:

```bash
make clean
```

### Flags utilizadas

```bash
gcc -Wall -O3 -fopenmp -fno-stack-protector \
    -foffload=nvptx-none="-misa=sm_80 -O3 -lm -fcf-protection=none -fno-stack-protector" \
    main.c ../utils/io_utils.c ../utils/math_utils.c -o kmeans_openmp_gpu -lm
```

Obs: GCC 13 só suporta até sm_80 no backend nvptx. A RTX 4050 é sm_89 mas roda PTX sm_80 por compatibilidade retroativa. As flags `-fno-stack-protector` e `-fcf-protection=none` são necessárias porque o PTX não tem suporte ao stack canary do Linux.

## 6. Saída Esperada

```text
Carregou 333 pontos com 4 features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Iniciando K-Means OpenMP GPU
K-means concluido com sucesso em 10 interacoes
Tempo total: 0.125462111 segundos
Tempo de offload/transferencia: 0.121175451 segundos
Clusters salvo: ../../data/processed/results_openmp_gpu.csv
```

## 7. Resultado Gerado

O arquivo de saída é:

```text
../../data/processed/results_openmp_gpu.csv
```

Com o mesmo formato das demais versões:

```csv
point_id,cluster_id
0,_
1,_
2,_
```

## 8. Resultados de Desempenho

| Versão | Tempo total (s) | Tempo de offload/transferência (s) | Iterações | Speedup |
|---|---:|---:|---:|---:|
| Sequencial | 0.000080000 | - | 9 | 1.00 |
| OpenMP GPU | 0.125462111 | 0.121175451 | 10 | 0.000638 |

## 9. Cálculo das Métricas

```text
speedup = tempo_sequencial / tempo_openmp_gpu
speedup = 0.000080000 / 0.125462111 ≈ 0.000638
```

O tempo de kernels (execução efetiva na GPU) é calculado como:
```text
tempo_kernels = tempo_total - tempo_offload
tempo_kernels = 0.125462111 - 0.121175451 = 0.004286660 s
```

A eficiência não é calculada da mesma forma que nas versões CPU, pois o número de threads é gerenciado automaticamente pelo runtime OpenMP (nvptx-none) com base no hardware disponível.

## 10. Discussão dos Resultados

- Os clusters gerados são idênticos aos da versão CUDA, e diferem do sequencial apenas no ponto ID 328. Isso é esperado: a ordem das operações atômicas em paralelo muda o acúmulo dos centróides da mesma forma nas duas versões GPU.

- O tempo de offload/transferência dominou com 96,6% do total, o mesmo padrão da versão CUDA (98,8%). Para 333 pontos, o custo de inicializar o contexto GPU e transferir dados pelo PCIe é muito maior que o tempo de computação em si.

- Comparando com CUDA: o tempo total do OpenMP GPU foi menor (0.1255s vs 0.1581s), mas o tempo dos kernels foi maior (0.0043s vs 0.0014s). O CUDA é mais eficiente nos kernels, mas tem mais overhead de alocação de memória com cudaMalloc.

- A implementação com diretivas foi bem mais simples que CUDA: sem escrever kernels, sem configurar blocos/grids, sem API CUDA. A estrutura do código ficou próxima ao sequencial.

## 11. Conclusão

Para o dataset Penguins (333 pontos), a versão GPU não trouxe ganho nenhum sobre a sequencial, o que era esperado dado o tamanho da entrada. O overhead de inicialização e comunicação GPU domina completamente o tempo de execução.

A comparação com CUDA mostrou que o OpenMP GPU é uma alternativa viável quando a simplicidade de código importa mais que o desempenho dos kernels em si.
