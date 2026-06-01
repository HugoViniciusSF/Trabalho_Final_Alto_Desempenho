# TO DO das Implementacoes

Este arquivo centraliza os checklists das implementacoes em `src/`.

## Sequencial

### Checklist

- [x] Ler arquivo CSV em C
- [x] Armazenar os dados em matriz numerica
- [x] Inicializar centroides
- [x] Calcular distancia euclidiana
- [x] Atualizar clusters
- [x] Definir criterio de parada

### Metricas

- [x] Medir tempo de execucao
- [x] Calcular speedup
- [x] Calcular eficiencia
- [ ] Avaliar escalabilidade forte
- [ ] Avaliar escalabilidade fraca
- [ ] Discutir overhead de comunicacao no MPI
- [ ] Discutir balanceamento de carga
- [ ] Comparar OpenMP GPU e CUDA
- [ ] Discutir impacto da transferencia CPU-GPU

## MPI + OpenMP

### Checklist

- [ ] Dividir dados entre processos MPI
- [ ] Paralelizar calculo de distancias com OpenMP
- [ ] Sincronizar somas parciais dos centroides com MPI
- [ ] Atualizar centroides globalmente
- [ ] Testar diferentes numeros de processos MPI
- [ ] Testar diferentes numeros de threads OpenMP

### Metricas

- [ ] Medir tempo de execucao
- [ ] Calcular speedup
- [ ] Calcular eficiencia
- [ ] Avaliar escalabilidade forte
- [ ] Avaliar escalabilidade fraca
- [ ] Discutir overhead de comunicacao no MPI
- [ ] Discutir balanceamento de carga
- [ ] Comparar OpenMP GPU e CUDA
- [ ] Discutir impacto da transferencia CPU-GPU

## OpenMP GPU

### Checklist

- [ ] Adaptar estruturas de dados para offload
- [ ] Enviar dados e centroides para GPU
- [ ] Paralelizar atribuicao dos pontos aos clusters
- [ ] Atualizar centroides
- [ ] Medir impacto da transferencia CPU-GPU

### Metricas

- [ ] Medir tempo de execucao
- [ ] Calcular speedup
- [ ] Calcular eficiencia
- [ ] Avaliar escalabilidade forte
- [ ] Avaliar escalabilidade fraca
- [ ] Discutir overhead de comunicacao no MPI
- [ ] Discutir balanceamento de carga
- [ ] Comparar OpenMP GPU e CUDA
- [ ] Discutir impacto da transferencia CPU-GPU

## CUDA

### Checklist

- [X] Criar kernel para calculo de distancias
- [x] Criar kernel ou rotina para atribuicao dos clusters
- [x] Atualizar centroides
- [x] Gerenciar alocacao e copia de memoria CPU-GPU
- [x] Medir tempo total e tempo de kernels

### Metricas

- [x] Medir tempo de execucao
- [ ] Calcular speedup
- [ ] Calcular eficiencia
- [ ] Avaliar escalabilidade forte
- [ ] Avaliar escalabilidade fraca
- [ ] Discutir overhead de comunicacao no MPI
- [ ] Discutir balanceamento de carga
- [ ] Comparar OpenMP GPU e CUDA
- [ ] Discutir impacto da transferencia CPU-GPU
