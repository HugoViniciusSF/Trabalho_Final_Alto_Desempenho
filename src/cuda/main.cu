#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Diz ao compilador C++ (CUDA) que estes ficheiros usam linguagem C padrão
extern "C" {
    #include "../utils/dataset_config.h"
    #include "../utils/io_utils.h"
    #include "../utils/math_utils.h"
}

extern "C" int run_kmeans_cuda(
    const double *dataset_host, int numero_de_pontos, int numero_de_clusters, 
    int numero_de_features, int iteracoes_maximas, double *centroides_host, 
    int *labels_host, double *tempo_kernels_out, double *tempo_transferencia_out
);

int main() {
    struct timespec start, end;
    double *dataset = NULL;
    int num_points = 0;
    
    if (load_dataset(CLEAN_DATA_PATH, &dataset, &num_points) == 0) {
        
        // Impressão exata solicitada (linhas 1 e 2)
        printf("Carregou %d pontos com %d features.\n", num_points, NUM_FEATURES);
        printf("Lendo as seguintes colunas: ");
        for (int i = 0; i < NUM_FEATURES; i++) {
            printf("%s%s", FEATURE_NAMES[i], (i == NUM_FEATURES - 1) ? "\n" : ", ");
        }
        
        double *centroids = (double *)malloc(NUM_CLUSTERS * NUM_FEATURES * sizeof(double));
        int *labels = (int *)malloc(num_points * sizeof(int));
        for (int i = 0; i < num_points; i++) labels[i] = -1;
        
        initialize_centroids(dataset, num_points, NUM_CLUSTERS, NUM_FEATURES, centroids);

        double tempo_kernels = 0.0;
        double tempo_transferencia = 0.0;

        clock_gettime(CLOCK_MONOTONIC, &start);
        
        run_kmeans_cuda(dataset, num_points, NUM_CLUSTERS, NUM_FEATURES, 100, 
                        centroids, labels, &tempo_kernels, &tempo_transferencia);
        
        clock_gettime(CLOCK_MONOTONIC, &end);

        double tempo_total = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        
        // Impressão exata solicitada (Tempos)
        printf("Tempo total: %.9f segundos\n", tempo_total);
        printf("Tempo dos kernels: %.9f segundos\n", tempo_kernels);
        printf("Tempo de transferencia CPU-GPU: %.9f segundos\n", tempo_transferencia);
        
        const char *output_file = "../../data/processed/results_cuda.csv";
        if (save_results(output_file, labels, num_points) == 0) {
            printf("Clusters salvo: %s\n", output_file);
        }
        
        free(centroids);
        free(labels);
        free(dataset);
    }
    return 0;
}