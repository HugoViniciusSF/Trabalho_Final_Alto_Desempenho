# Resultados do Benchmark K-Means Expanded

**Número de execuções por experimento:** 10

## Tabela de Medianas

| Experimento | Mediana de Tempo |
|---|---:|
| Sequencial | 0.146492 |
| MPI+OpenMP (1 proc, 1 thread) | 0.168176 |
| MPI+OpenMP (1 proc, 2 thread) | 0.088089 |
| MPI+OpenMP (1 proc, 4 thread) | 0.090962 |
| MPI+OpenMP (2 proc, 1 thread) | 0.085023 |
| MPI+OpenMP (2 proc, 2 thread) | 0.045628 |
| MPI+OpenMP (2 proc, 4 thread) | 0.053292 |
| MPI+OpenMP (4 proc, 1 thread) | 0.042947 |
| MPI+OpenMP (4 proc, 2 thread) | 0.046631 |
| MPI+OpenMP (4 proc, 4 thread) | 0.037109 |
| OpenMP GPU | 2.386281 |
| CUDA | 0.245684 |

## Tempo de cada Execução

### Sequencial
- Tempos obtidos: 0.148827, 0.145576, 0.146721, 0.146264, 0.144600, 0.144540, 0.159671, 0.156703, 0.146212, 0.149962

### MPI+OpenMP (1 proc, 1 thread)
- Tempos obtidos: 0.165142, 0.165191, 0.168784, 0.169052, 0.169184, 0.171642, 0.167567, 0.178024, 0.166786, 0.165249

### MPI+OpenMP (1 proc, 2 thread)
- Tempos obtidos: 0.088856, 0.086989, 0.087806, 0.088685, 0.087808, 0.088515, 0.088138, 0.088578, 0.088041, 0.088025

### MPI+OpenMP (1 proc, 4 thread)
- Tempos obtidos: 0.096035, 0.087906, 0.088421, 0.093663, 0.092001, 0.092961, 0.091260, 0.090664, 0.089247, 0.090385

### MPI+OpenMP (2 proc, 1 thread)
- Tempos obtidos: 0.084051, 0.083861, 0.092238, 0.084653, 0.085233, 0.085601, 0.086945, 0.087413, 0.083916, 0.084812

### MPI+OpenMP (2 proc, 2 thread)
- Tempos obtidos: 0.044794, 0.045251, 0.057585, 0.045528, 0.045476, 0.045728, 0.065690, 0.045079, 0.046092, 0.047182

### MPI+OpenMP (2 proc, 4 thread)
- Tempos obtidos: 0.049567, 0.059582, 0.056552, 0.053170, 0.050739, 0.046665, 0.053414, 0.053516, 0.048715, 0.054049

### MPI+OpenMP (4 proc, 1 thread)
- Tempos obtidos: 0.042449, 0.042879, 0.043014, 0.043619, 0.046213, 0.043431, 0.042664, 0.042335, 0.043331, 0.042693

### MPI+OpenMP (4 proc, 2 thread)
- Tempos obtidos: 0.046754, 0.045764, 0.048708, 0.047978, 0.051065, 0.046508, 0.045650, 0.046775, 0.044347, 0.041475

### MPI+OpenMP (4 proc, 4 thread)
- Tempos obtidos: 0.041748, 0.043684, 0.042054, 0.033580, 0.034731, 0.039487, 0.032845, 0.056426, 0.030880, 0.033251

### OpenMP GPU
- Tempos obtidos: 2.388988, 2.396884, 2.387305, 2.364929, 2.360228, 2.392746, 2.384696, 2.408195, 2.377811, 2.385258

### CUDA
- Tempos obtidos: 0.258347, 0.234908, 0.240865, 0.242228, 0.260600, 0.249139, 0.240286, 0.253542, 0.257129, 0.234279

