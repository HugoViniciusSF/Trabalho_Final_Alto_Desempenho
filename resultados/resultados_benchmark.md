# Resultados do Benchmark K-Means

**Número de execuções por experimento:** 10

## Tabela de Medianas

| Experimento | Mediana de Tempo (s) |
|---|---:|
| Sequencial | 0.000027 |
| MPI+OpenMP (1 proc, 1 thread) | 0.000096 |
| MPI+OpenMP (1 proc, 2 thread) | 0.000090 |
| MPI+OpenMP (1 proc, 4 thread) | 0.000597 |
| MPI+OpenMP (2 proc, 1 thread) | 0.000099 |
| MPI+OpenMP (2 proc, 2 thread) | 0.008548 |
| MPI+OpenMP (2 proc, 4 thread) | 0.001630 |
| MPI+OpenMP (4 proc, 1 thread) | 0.000125 |
| MPI+OpenMP (4 proc, 2 thread) | 0.036334 |
| MPI+OpenMP (4 proc, 4 thread) | 0.039080 |
| OpenMP GPU | 0.169138 |
| CUDA | 0.119166 |

## Tempo de cada Execução

### Sequencial
- Tempos obtidos: 0.000026, 0.000027, 0.000124, 0.000027, 0.000027, 0.000027, 0.000027, 0.000027, 0.000027, 0.000026

### MPI+OpenMP (1 proc, 1 thread)
- Tempos obtidos: 0.000063, 0.000106, 0.000089, 0.000069, 0.000113, 0.000120, 0.000103, 0.000074, 0.000113, 0.000066

### MPI+OpenMP (1 proc, 2 thread)
- Tempos obtidos: 0.000089, 0.000084, 0.000091, 0.000088, 0.000093, 0.000116, 0.000086, 0.000105, 0.000118, 0.000056

### MPI+OpenMP (1 proc, 4 thread)
- Tempos obtidos: 0.000695, 0.000596, 0.000652, 0.000352, 0.000377, 0.000715, 0.000496, 0.000598, 0.001177, 0.000412

### MPI+OpenMP (2 proc, 1 thread)
- Tempos obtidos: 0.000127, 0.000095, 0.000091, 0.000142, 0.000099, 0.000100, 0.000135, 0.000130, 0.000092, 0.000072

### MPI+OpenMP (2 proc, 2 thread)
- Tempos obtidos: 0.000252, 0.023528, 0.014577, 0.000230, 0.009626, 0.000267, 0.007471, 0.000194, 0.010735, 0.018022

### MPI+OpenMP (2 proc, 4 thread)
- Tempos obtidos: 0.002935, 0.001869, 0.001731, 0.001686, 0.000949, 0.002077, 0.001376, 0.001575, 0.001317, 0.001369

### MPI+OpenMP (4 proc, 1 thread)
- Tempos obtidos: 0.000139, 0.000102, 0.000537, 0.000103, 0.000100, 0.000110, 0.000160, 0.000102, 0.000174, 0.000256

### MPI+OpenMP (4 proc, 2 thread)
- Tempos obtidos: 0.007644, 0.039538, 0.000255, 0.042095, 0.028020, 0.033130, 0.044958, 0.017480, 0.041066, 0.040952

### MPI+OpenMP (4 proc, 4 thread)
- Tempos obtidos: 0.043369, 0.047750, 0.034791, 0.055488, 0.025018, 0.057397, 0.025549, 0.071676, 0.007707, 0.013695

### OpenMP GPU
- Tempos obtidos: 0.208385, 0.164291, 0.161799, 0.172063, 0.191617, 0.161714, 0.170799, 0.161760, 0.174420, 0.167477

### CUDA
- Tempos obtidos: 0.120280, 0.118987, 0.151674, 0.116563, 0.120988, 0.114695, 0.133549, 0.118756, 0.119345, 0.118535

