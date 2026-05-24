#include <stdio.h>
#include <stdlib.h>
#include "../utils/dataset_config.h"
#include "../utils/io_utils.h"
#include "../utils/math_utils.h"

int main() {
    double *dataset = NULL;
    int num_points = 0;
    
    if (load_dataset(CLEAN_DATA_PATH, &dataset, &num_points) == 0) {
        printf("Carregou %d pontos com %d features.\n", num_points, NUM_FEATURES);
        
        double *centroids = (double *)malloc(NUM_CLUSTERS * NUM_FEATURES * sizeof(double));
        if (!centroids) {
            perror("Erro ao alocar memória para os centroides");
            free(dataset);
            return -1;
        }

        printf("A inicializar os %d centroides iniciais...\n", NUM_CLUSTERS);
        initialize_centroids(dataset, num_points, NUM_CLUSTERS, NUM_FEATURES, centroids);

        // Validar centroids
        for (int i = 0; i < NUM_CLUSTERS; i++) {
            printf("Centroide %d: [", i);
            for (int j = 0; j < NUM_FEATURES; j++) {
                printf("%.4f", centroids[i * NUM_FEATURES + j]);
                if (j < NUM_FEATURES - 1) printf(", ");
            }
            printf("]\n");
        }
        free(centroids);
        free(dataset);
    }
    return 0;
}