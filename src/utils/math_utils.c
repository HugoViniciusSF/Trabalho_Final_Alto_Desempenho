#include "math_utils.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
    int *cluster_sizes = (int *)calloc(k, sizeof(int));                         // Conta quantos pinguins estão em cada cluster
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

#ifdef USE_MPI
#include <mpi.h>
#ifdef _OPENMP
#include <omp.h>
#endif

void run_kmeans_mpi_openmp(const double *dataset, int num_points, int k, int num_features, int max_iterations, double *centroids, int *labels, int rank, int size) {
    int *cluster_sizes = (int *)calloc(k, sizeof(int));
    double *new_centroids = (double *)calloc(k * num_features, sizeof(double));

    int *global_cluster_sizes = (int *)calloc(k, sizeof(int));
    double *global_new_centroids = (double *)calloc(k * num_features, sizeof(double));

    int iterations = 0;
    int converged = 0;

    if (rank == 0)
        printf("Iniciando K-Means MPI + OpenMP\n");

    while (iterations < max_iterations && !converged) {
        int local_converged = 1;

        for (int j = 0; j < k; j++) {
            cluster_sizes[j] = 0;
            for (int f = 0; f < num_features; f++) {
                new_centroids[j * num_features + f] = 0.0;
            }
        }

#pragma omp parallel
        {
            int *thread_cluster_sizes = (int *)calloc(k, sizeof(int));
            double *thread_new_centroids = (double *)calloc(k * num_features, sizeof(double));
            int thread_converged = 1;

#pragma omp for
            for (int i = 0; i < num_points; i++) {
                double min_dist = DBL_MAX;
                int best_cluster = -1;

                for (int j = 0; j < k; j++) {
                    double dist = euclidean_distance(&dataset[i * num_features], &centroids[j * num_features], num_features);
                    if (dist < min_dist) {
                        min_dist = dist;
                        best_cluster = j;
                    }
                }

                if (labels[i] != best_cluster) {
                    labels[i] = best_cluster;
                    thread_converged = 0;
                }

                thread_cluster_sizes[best_cluster]++;
                for (int f = 0; f < num_features; f++) {
                    thread_new_centroids[best_cluster * num_features + f] += dataset[i * num_features + f];
                }
            }

#pragma omp critical
            {
                if (thread_converged == 0)
                    local_converged = 0;
                for (int j = 0; j < k; j++) {
                    cluster_sizes[j] += thread_cluster_sizes[j];
                    for (int f = 0; f < num_features; f++) {
                        new_centroids[j * num_features + f] += thread_new_centroids[j * num_features + f];
                    }
                }
            }
            free(thread_cluster_sizes);
            free(thread_new_centroids);
        }

        MPI_Allreduce(&local_converged, &converged, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);
        MPI_Allreduce(cluster_sizes, global_cluster_sizes, k, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(new_centroids, global_new_centroids, k * num_features, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        for (int j = 0; j < k; j++) {
            if (global_cluster_sizes[j] > 0) {
                for (int f = 0; f < num_features; f++) {
                    centroids[j * num_features + f] = global_new_centroids[j * num_features + f] / global_cluster_sizes[j];
                }
            }
        }

        iterations++;
    }

    if (rank == 0)
        printf("K-means concluido com sucesso em %d interacoes\n", iterations);

    free(cluster_sizes);
    free(new_centroids);
    free(global_cluster_sizes);
    free(global_new_centroids);
}
#endif
