# TO DO das Implementações

Este arquivo centraliza os checklists das implementações em `src/`.

## Sequencial

### Checklist

- [x] Ler arquivo CSV em C
- [x] Armazenar os dados em matriz numérica
- [x] Inicializar centroides
- [x] Calcular distância euclidiana
- [x] Atualizar clusters
- [x] Definir critério de parada

### Métricas

- [x] Medir tempo de execução
- [x] Calcular speedup
- [x] Calcular eficiência
- [x] Usar como baseline das versões paralelas

## MPI + OpenMP

### Checklist

- [x] Dividir dados entre processos MPI
- [x] Paralelizar cálculo de distâncias com OpenMP
- [x] Sincronizar somas parciais dos centroides com MPI
- [x] Atualizar centroides globalmente
- [x] Testar diferentes números de processos MPI
- [x] Testar diferentes números de threads OpenMP

### Métricas

- [x] Medir tempo de execução
- [x] Calcular speedup
- [x] Calcular eficiência
- [x] Avaliar escalabilidade forte
- [ ] Avaliar escalabilidade fraca
- [x] Discutir overhead de comunicação no MPI
- [x] Discutir balanceamento de carga

## OpenMP GPU

### Checklist

- [x] Adaptar estruturas de dados para offload
- [x] Enviar dados e centroides para GPU
- [x] Paralelizar atribuição dos pontos aos clusters
- [x] Atualizar centroides
- [x] Medir impacto da transferência CPU-GPU

### Métricas

- [x] Medir tempo de execução
- [x] Calcular speedup
- [ ] Calcular eficiência
- [x] Comparar OpenMP GPU e CUDA
- [x] Discutir impacto da transferência CPU-GPU

## CUDA

### Checklist

- [x] Criar kernel para cálculo de distâncias
- [x] Criar kernel ou rotina para atribuição dos clusters
- [x] Atualizar centroides
- [x] Gerenciar alocação e cópia de memória CPU-GPU
- [x] Medir tempo total e tempo de kernels

### Métricas

- [x] Medir tempo de execução
- [x] Calcular speedup
- [x] Calcular eficiência
- [x] Comparar OpenMP GPU e CUDA
- [x] Discutir impacto da transferência CPU-GPU

## Resultados e Análise

### Checklist

- [x] Criar script para comparar clusters gerados
- [x] Comparar clusters do sequencial com CUDA
- [x] Comparar clusters do sequencial com OpenMP GPU
- [x] Comparar clusters do sequencial com MPI + OpenMP
- [x] Comparar clusters do CUDA com OpenMP GPU
- [x] Comparar clusters do MPI + OpenMP com CUDA
- [x] Comparar clusters do MPI + OpenMP com OpenMP GPU
- [x] Criar tabela comparativa geral dos resultados
- [x] Atualizar comparativo geral com MPI + OpenMP
- [x] Avaliar escalabilidade forte do MPI + OpenMP
- [ ] Avaliar escalabilidade fraca
