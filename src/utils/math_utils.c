#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
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

void run_kmeans(const double *dataset, int num_points, int k, int num_features, int max_iterations, double *centroids, int *labels) {
    int *cluster_sizes = (int *)calloc(k, sizeof(int)); // Conta quantos pinguins estão em cada cluster
    double *new_centroids = (double *)calloc(k * num_features, sizeof(double)); // Acumula as somas das features

    int iterations = 0;
    int converged = 0; // Critério de Parada (0 = Não convergiu, 1 = Convergiu)

    printf("Iniciando K-Means\n");

    // Primeiro critério de parada - Max Iterações
    while (iterations < max_iterations && !converged) {
        converged = 1; // Assumimos que convergiu, mudamos para zero se algum ponto mudar de cluster

        // Zerar os acumuladores para a nova iteração
        for (int j = 0; j < k; j++) {
            cluster_sizes[j] = 0;
            for (int f = 0; f < num_features; f++) {
                new_centroids[j * num_features + f] = 0.0;
            }
        }

        // Ponto de paralelização
        for (int i = 0; i < num_points; i++) {
            double min_dist = DBL_MAX;
            int best_cluster = -1;

            // Calcula a distância euclidiana para todos os centroides
            for (int j = 0; j < k; j++) {
                // Passamos os ponteiros usando aritmética 1D
                double dist = euclidean_distance(&dataset[i * num_features], &centroids[j * num_features], num_features);
                if (dist < min_dist) {
                    min_dist = dist;
                    best_cluster = j;
                }
            }

            // Segundo critério de parada, se o ponto mudou de cluster, o algoritmo ainda não convergiu
            if (labels[i] != best_cluster) {
                labels[i] = best_cluster;
                converged = 0;
            }
            // Soma este ponto aos acumuladores do cluster vencedor
            cluster_sizes[best_cluster]++;
            for (int f = 0; f < num_features; f++) {
                new_centroids[best_cluster * num_features + f] += dataset[i * num_features + f];
            }
        }

        for (int j = 0; j < k; j++) {
            if (cluster_sizes[j] > 0) {
                for (int f = 0; f < num_features; f++) {
                    // O novo centro de massa é a média das somas (Soma / Quantidade)
                    centroids[j * num_features + f] = new_centroids[j * num_features + f] / cluster_sizes[j];
                }
            }
        }

        iterations++;
    }
    printf("K-means concluido com sucesso em %d interacoes\n", iterations);
    free(cluster_sizes);
    free(new_centroids);
}