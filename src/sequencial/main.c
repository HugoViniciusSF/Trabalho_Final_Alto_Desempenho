#include <stdio.h>
#include <stdlib.h>
#include "../utils/io_utils.h"
#include "../utils/dataset_config.h"

int main() {
    double *dataset = NULL;
    int num_points = 0;
    
    if (load_dataset(CLEAN_DATA_PATH, &dataset, &num_points) == 0) {
        printf("Carregou %d pontos com %d features.\n", num_points, NUM_FEATURES);
    
        ////////////////////////////////////////////
        printf("Primeiras 5 linhas do Dataset\n");
        int limit = (num_points < 5) ? num_points : 5;
        for (int i = 0; i < limit; i++) {
            printf("Ponto %d: [", i);
            
            for (int j = 0; j < NUM_FEATURES; j++) {
                printf("%.4f", dataset[i * NUM_FEATURES + j]);
                
                if (j < NUM_FEATURES - 1) {
                    printf(", ");
                }
            }
            printf("]\n");
        }
        printf("=====================================\n");
        ////////////////////////////////////////////    
        free(dataset);
    }
    return 0;
}