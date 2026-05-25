#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../utils/dataset_config.h"
#include "../utils/io_utils.h"
#include "../utils/math_utils.h"

int main() {
    struct timespec start, end;
    double *dataset = NULL;
    int num_points = 0;
    
    if (load_dataset(CLEAN_DATA_PATH, &dataset, &num_points) == 0) {
        printf("Carregou %d pontos com %d features.\n", num_points, NUM_FEATURES);
        printf("Lendo as seguintes colunas: ");
        for (int i = 0; i < NUM_FEATURES; i++) {
            printf("%s%s", FEATURE_NAMES[i], (i == NUM_FEATURES - 1) ? "\n" : ", ");
        }
        double *centroids = (double *)malloc(NUM_CLUSTERS * NUM_FEATURES * sizeof(double));
        int *labels = (int *)malloc(num_points * sizeof(int));
        for (int i = 0; i < num_points; i++) labels[i] = -1;
        initialize_centroids(dataset, num_points, NUM_CLUSTERS, NUM_FEATURES, centroids);

        // Medir o tempo apenas do run_kmeans, se necessário medir IO precisa mudar a posição
        clock_gettime(CLOCK_MONOTONIC, &start);
        run_kmeans(dataset, num_points, NUM_CLUSTERS, NUM_FEATURES, 100, centroids, labels);
        // Fim da medição
        clock_gettime(CLOCK_MONOTONIC, &end);

        double tempo_exec = (end.tv_sec - start.tv_sec) + 
                            (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Wall Clock Time: %.9f segundos\n", tempo_exec); // Tempo de execução sequencial já está em 10^-4
        const char *output_file = "../../data/processed/results_sequencial.csv";
        if (save_results(output_file, labels, num_points) == 0) {
            printf("Clusters salvo: %s\n", output_file);
        }
        free(centroids);
        free(labels);
        free(dataset);
    }
    return 0;
}