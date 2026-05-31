#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <cuda_runtime.h>

// Funções auxiliares de tempo
static inline void obter_tempo(struct timespec *ts) { clock_gettime(CLOCK_MONOTONIC, ts); }
static inline double calcular_diferenca(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

// ---------------------------------------------------------------------------------
// KERNEL 1: Atribui o ponto ao cluster e SOMA os valores usando operações atómicas
// ---------------------------------------------------------------------------------
__global__ void kernel_atribuir_e_somar(
    const double *dataset_no_device, int numero_de_pontos, int numero_de_clusters, 
    int numero_de_features, const double *centroides_no_device, 
    int *labels_no_device, int *ocorreram_mudancas_no_device,
    double *somas_dos_centroides_device, int *tamanho_dos_clusters_device) 
{
    int id_do_ponto = blockIdx.x * blockDim.x + threadIdx.x;

    if (id_do_ponto < numero_de_pontos) {
        double distancia_minima = DBL_MAX;
        int id_cluster_vencedor = -1;

        // 1. Encontrar o centroide mais próximo
        for (int j = 0; j < numero_de_clusters; j++) {
            double soma_distancias = 0.0;
            for (int f = 0; f < numero_de_features; f++) {
                double diferenca = dataset_no_device[id_do_ponto * numero_de_features + f] - 
                                   centroides_no_device[j * numero_de_features + f];
                soma_distancias += diferenca * diferenca;
            }
            double distancia = sqrt(soma_distancias);

            if (distancia < distancia_minima) {
                distancia_minima = distancia;
                id_cluster_vencedor = j;
            }
        }

        // 2. Verificar se mudou e atualizar label
        if (labels_no_device[id_do_ponto] != id_cluster_vencedor) {
            labels_no_device[id_do_ponto] = id_cluster_vencedor;
            *ocorreram_mudancas_no_device = 1; 
        }

        // 3. Somar de forma segura (Atomic) para evitar Condição de Corrida
        atomicAdd(&tamanho_dos_clusters_device[id_cluster_vencedor], 1);
        for (int f = 0; f < numero_de_features; f++) {
            atomicAdd(&somas_dos_centroides_device[id_cluster_vencedor * numero_de_features + f], 
                      dataset_no_device[id_do_ponto * numero_de_features + f]);
        }
    }
}

// ---------------------------------------------------------------------------------
// KERNEL 2: Divide a soma pela quantidade para achar a nova média (novo centroide)
// ---------------------------------------------------------------------------------
__global__ void kernel_calcular_medias(
    int numero_de_clusters, int numero_de_features, 
    double *centroides_no_device, double *somas_dos_centroides_device, 
    int *tamanho_dos_clusters_device) 
{
    // Apenas 1 thread por cluster (Ex: 3 threads totais)
    int id_do_cluster = blockIdx.x * blockDim.x + threadIdx.x;

    if (id_do_cluster < numero_de_clusters) {
        int quantidade_de_pontos = tamanho_dos_clusters_device[id_do_cluster];

        // Se o cluster não estiver vazio, calcula a nova média
        if (quantidade_de_pontos > 0) {
            for (int f = 0; f < numero_de_features; f++) {
                centroides_no_device[id_do_cluster * numero_de_features + f] = 
                    somas_dos_centroides_device[id_do_cluster * numero_de_features + f] / quantidade_de_pontos;
            }
        }

        // Zera os acumuladores imediatamente para a próxima iteração do K-means
        tamanho_dos_clusters_device[id_do_cluster] = 0;
        for (int f = 0; f < numero_de_features; f++) {
            somas_dos_centroides_device[id_do_cluster * numero_de_features + f] = 0.0;
        }
    }
}

// ---------------------------------------------------------------------------------
// FUNÇÃO GERENTE
// ---------------------------------------------------------------------------------
extern "C" int run_kmeans_cuda(
    const double *dataset_host, int numero_de_pontos, int numero_de_clusters, 
    int numero_de_features, int iteracoes_maximas, double *centroides_host, 
    int *labels_host, double *tempo_kernels_out, double *tempo_transferencia_out) 
{
    struct timespec t_inicial, t_final;
    double acumulador_kernels = 0.0, acumulador_transferencia = 0.0;

    // Alocações na Placa Gráfica
    obter_tempo(&t_inicial);
    double *dataset_no_device, *centroides_no_device, *somas_dos_centroides_device;
    int *labels_no_device, *ocorreram_mudancas_no_device, *tamanho_dos_clusters_device;

    size_t bytes_dataset = numero_de_pontos * numero_de_features * sizeof(double);
    size_t bytes_centroides = numero_de_clusters * numero_de_features * sizeof(double);
    size_t bytes_labels = numero_de_pontos * sizeof(int);
    size_t bytes_tamanho_clusters = numero_de_clusters * sizeof(int);

    cudaMalloc((void**)&dataset_no_device, bytes_dataset);
    cudaMalloc((void**)&centroides_no_device, bytes_centroides);
    cudaMalloc((void**)&somas_dos_centroides_device, bytes_centroides);
    cudaMalloc((void**)&labels_no_device, bytes_labels);
    cudaMalloc((void**)&tamanho_dos_clusters_device, bytes_tamanho_clusters);
    cudaMalloc((void**)&ocorreram_mudancas_no_device, sizeof(int));

    // Inicializa os acumuladores com zero diretamente na placa de vídeo
    cudaMemset(somas_dos_centroides_device, 0, bytes_centroides);
    cudaMemset(tamanho_dos_clusters_device, 0, bytes_tamanho_clusters);

    // Envio inicial (Uma ÚNICA vez antes do laço)
    cudaMemcpy(dataset_no_device, dataset_host, bytes_dataset, cudaMemcpyHostToDevice);
    cudaMemcpy(centroides_no_device, centroides_host, bytes_centroides, cudaMemcpyHostToDevice);
    cudaMemcpy(labels_no_device, labels_host, bytes_labels, cudaMemcpyHostToDevice);
    
    obter_tempo(&t_final);
    acumulador_transferencia += calcular_diferenca(t_inicial, t_final);

    // Configuração de blocos
    int tamanho_do_bloco_pinguins = 256; 
    int numero_de_blocos_pinguins = (numero_de_pontos + tamanho_do_bloco_pinguins - 1) / tamanho_do_bloco_pinguins;
    
    // O segundo kernel só precisa de 3 threads (1 para cada cluster)
    int tamanho_do_bloco_medias = 32; 
    int numero_de_blocos_medias = 1; 

    int iteracao_atual = 0;
    int convergencia_alcancada = 0;

    printf("Iniciando K-Means (CUDA com AtomicAdd)\n");

    while (iteracao_atual < iteracoes_maximas && !convergencia_alcancada) {
        int mudancas_nesta_iteracao = 0;
        
        // Envia apenas a "bandeira" de mudança (4 bytes)
        obter_tempo(&t_inicial);
        cudaMemcpy(ocorreram_mudancas_no_device, &mudancas_nesta_iteracao, sizeof(int), cudaMemcpyHostToDevice);
        obter_tempo(&t_final);
        acumulador_transferencia += calcular_diferenca(t_inicial, t_final);

        // ================= KERNELS =================
        cudaDeviceSynchronize(); 
        obter_tempo(&t_inicial);
        
        // Passo 1: Atribuir e Acumular
        kernel_atribuir_e_somar<<<numero_de_blocos_pinguins, tamanho_do_bloco_pinguins>>>(
            dataset_no_device, numero_de_pontos, numero_de_clusters, numero_de_features, 
            centroides_no_device, labels_no_device, ocorreram_mudancas_no_device,
            somas_dos_centroides_device, tamanho_dos_clusters_device
        );

        // Passo 2: Calcular a Média (Novos Centroides)
        kernel_calcular_medias<<<numero_de_blocos_medias, tamanho_do_bloco_medias>>>(
            numero_de_clusters, numero_de_features, 
            centroides_no_device, somas_dos_centroides_device, tamanho_dos_clusters_device
        );

        cudaDeviceSynchronize(); 
        obter_tempo(&t_final);
        acumulador_kernels += calcular_diferenca(t_inicial, t_final);
        // ===========================================

        // Traz de volta apenas a "bandeira" de mudança para verificar a paragem
        obter_tempo(&t_inicial);
        cudaMemcpy(&mudancas_nesta_iteracao, ocorreram_mudancas_no_device, sizeof(int), cudaMemcpyDeviceToHost);
        obter_tempo(&t_final);
        acumulador_transferencia += calcular_diferenca(t_inicial, t_final);

        if (mudancas_nesta_iteracao == 0) convergencia_alcancada = 1;
        iteracao_atual++;
    }

    printf("K-means (CUDA) concluido com sucesso em %d interacoes\n", iteracao_atual);
    fflush(stdout); 

    // No final, trazemos a resposta definitiva (labels e novos centroides) de volta para a CPU
    obter_tempo(&t_inicial);
    cudaMemcpy(labels_host, labels_no_device, bytes_labels, cudaMemcpyDeviceToHost);
    cudaMemcpy(centroides_host, centroides_no_device, bytes_centroides, cudaMemcpyDeviceToHost);
    
    cudaFree(dataset_no_device); cudaFree(centroides_no_device);
    cudaFree(somas_dos_centroides_device); cudaFree(labels_no_device); 
    cudaFree(tamanho_dos_clusters_device); cudaFree(ocorreram_mudancas_no_device);
    obter_tempo(&t_final);
    acumulador_transferencia += calcular_diferenca(t_inicial, t_final);

    *tempo_kernels_out = acumulador_kernels;
    *tempo_transferencia_out = acumulador_transferencia;

    return iteracao_atual;
}