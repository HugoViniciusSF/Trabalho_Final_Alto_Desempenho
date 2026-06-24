# Utilitários compartilhados

A pasta `src/utils` reúne funções e configurações usadas pelas diferentes versões do K-means. O objetivo é evitar repetição de código entre as implementações sequencial, MPI + OpenMP, OpenMP GPU e CUDA.

## Arquivos

| Arquivo | Função |
|---|---|
| `dataset_config.h` | Define o caminho do CSV tratado, o número de clusters, a quantidade de features e os nomes das colunas usadas. |
| `io_utils.c` | Implementa a leitura do dataset e a escrita dos clusters gerados. |
| `io_utils.h` | Declara as funções de entrada e saída usadas pelas versões do K-means. |
| `math_utils.c` | Implementa funções matemáticas e o fluxo base do K-means em CPU. Também contém a versão auxiliar usada pelo MPI + OpenMP. |
| `math_utils.h` | Declara as funções matemáticas usadas pelas implementações. |

## `dataset_config.h`

Esse arquivo centraliza a configuração do dataset usado no projeto.

Configurações definidas:

| Configuração | Valor |
|---|---|
| `CLEAN_DATA_PATH` | `../../data/processed/penguins_clean.csv` |
| `NUM_CLUSTERS` | `3` |
| `NUM_FEATURES` | `4` |

Features usadas:

- `bill_length_mm`
- `bill_depth_mm`
- `flipper_length_mm`
- `body_mass_g`

Essas definições permitem que todas as versões leiam as mesmas colunas e usem a mesma quantidade de clusters.

## `io_utils.c`

Esse arquivo implementa duas funções principais.

### `load_dataset`

```c
int load_dataset(const char *filename, double **dataset, int *num_points);
```

Responsabilidades:

- abrir o arquivo CSV informado;
- identificar, pelo cabeçalho, as colunas definidas em `FEATURE_NAMES`;
- carregar apenas as quatro features usadas pelo K-means;
- armazenar os dados em um array `double` unidimensional e contíguo;
- preencher `num_points` com a quantidade de registros carregados.

O armazenamento em array 1D segue o formato:

```text
dataset[ponto * NUM_FEATURES + feature]
```

Esse formato facilita o reuso dos dados nas versões em CPU e GPU.

### `save_results`

```c
int save_results(const char *filename, int *labels, int num_points);
```

Responsabilidades:

- criar um CSV de saída;
- salvar o identificador do ponto;
- salvar o cluster atribuído a cada ponto.

Formato gerado:

```csv
point_id,cluster_id
0,1
1,0
```

## `math_utils.c`

Esse arquivo concentra as funções matemáticas compartilhadas.

### `euclidean_distance`

```c
double euclidean_distance(const double *p1, const double *p2, int num_features);
```

Calcula a distância euclidiana entre dois pontos usando as features numéricas selecionadas.

### `initialize_centroids`

```c
void initialize_centroids(const double *dataset, int num_points, int k, int num_features, double *centroids);
```

Inicializa os centroides escolhendo `k` pontos únicos do dataset.

Detalhes da implementação:

- usa `srand(42)` para manter a mesma semente;
- evita escolher o mesmo ponto mais de uma vez;
- copia as features dos pontos escolhidos para o array de centroides.

Essa escolha ajuda a manter as versões comparáveis, pois todas partem dos mesmos centroides iniciais.

### `run_kmeans`

```c
void run_kmeans(
    const double *dataset,
    int num_points,
    int k,
    int num_features,
    int max_iterations,
    double *centroids,
    int *labels
);
```

Executa o K-means em CPU de forma sequencial.

Fluxo:

1. calcula a distância de cada ponto até cada centroide;
2. atribui o ponto ao cluster mais próximo;
3. acumula as somas das features por cluster;
4. recalcula os centroides pela média dos pontos;
5. repete até não haver mudança de cluster ou atingir o limite de iterações.

Essa função é usada pela versão sequencial.

### `run_kmeans_mpi_openmp`

```c
void run_kmeans_mpi_openmp(
    const double *dataset,
    int num_points,
    int k,
    int num_features,
    int max_iterations,
    double *centroids,
    int *labels,
    int rank,
    int size
);
```

Essa função é compilada quando `USE_MPI` está definido.

Ela executa a parte local do K-means na versão MPI + OpenMP:

- usa OpenMP para paralelizar a atribuição dos pontos locais aos clusters;
- usa acumuladores por thread para reduzir conflitos;
- combina os acumuladores locais dentro do processo;
- usa `MPI_Allreduce` para sincronizar convergência, tamanhos dos clusters e somas dos centroides entre processos;
- atualiza os centroides globais em todos os processos.

## Relação com as versões do projeto

| Versão | Uso de `src/utils` |
|---|---|
| Sequencial | Usa `load_dataset`, `initialize_centroids`, `run_kmeans` e `save_results`. |
| MPI + OpenMP | Usa `load_dataset`, `initialize_centroids`, `run_kmeans_mpi_openmp` e `save_results`. |
| OpenMP GPU | Usa `load_dataset`, `initialize_centroids`, `save_results` e implementa o laço principal com offload no próprio `main.c`. |
| CUDA | Usa `load_dataset`, `initialize_centroids`, `save_results` e implementa os kernels em `cuda_kmeans.cu`. |

## Observação

Os utilitários garantem que as versões usem o mesmo arquivo de entrada, as mesmas features e o mesmo formato de saída. Isso permite comparar os tempos e os clusters gerados pelas diferentes implementações.
