#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "math_utils.h"

double euclidean_distance(const double *p1, const double *p2, int num_features) {
    double sum = 0.0;
    for (int i = 0; i < num_features; i++) {
        double diff = p1[i] - p2[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

void initialize_centroids(const double *dataset, int num_points, int k, int num_features, double *centroids) {
    // Fixar a semente para garantir que todas as versões selecionem o mesmo centroid
    srand(42); 
    int *chosen_indices = (int *)malloc(k * sizeof(int));
    for (int i = 0; i < k; i++) {
        int random_index;
        int is_unique;
        // Garante que não escolhemos o mesmo pinguim duas vezes para centroides diferentes
        do {
            is_unique = 1;
            random_index = rand() % num_points;
            
            for (int j = 0; j < i; j++) {
                if (chosen_indices[j] == random_index) {
                    is_unique = 0;
                    break;
                }
            }
        } while (!is_unique);    
        chosen_indices[i] = random_index;
        
        // Copia as features do ponto escolhido para a matriz de centroides
        for (int f = 0; f < num_features; f++) {
            // Mapeamento achatado (índice do centroide * total de features + feature atual)
            centroids[i * num_features + f] = dataset[random_index * num_features + f];
        }
    }
    
    free(chosen_indices);
}