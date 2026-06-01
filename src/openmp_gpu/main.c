#include <float.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/dataset_config.h"
#include "../utils/io_utils.h"
#include "../utils/math_utils.h"

static int run_kmeans_openmp_gpu(const double *dataset, int num_points, int k, int num_features,
                                 int max_iterations, double *centroids, int *labels,
                                 double *transfer_time) {
    int data_size = num_points * num_features;
    int centroid_size = k * num_features;
    int *cluster_sizes = (int *)calloc(k, sizeof(int));
    double *new_centroids = (double *)calloc(centroid_size, sizeof(double));
    int iterations = 0;
    int changed = 1;
    double t0;

    if (!cluster_sizes || !new_centroids) {
        free(cluster_sizes);
        free(new_centroids);
        return -1;
    }

    *transfer_time = 0.0;

    t0 = omp_get_wtime();
    #pragma omp target enter data map(to: dataset[0:data_size], centroids[0:centroid_size], labels[0:num_points]) \
                                  map(alloc: cluster_sizes[0:k], new_centroids[0:centroid_size])
    *transfer_time += omp_get_wtime() - t0;

    while (iterations < max_iterations && changed) {
        changed = 0;

        #pragma omp target teams distribute parallel for
        for (int i = 0; i < k; i++) {
            cluster_sizes[i] = 0;
        }

        #pragma omp target teams distribute parallel for
        for (int i = 0; i < centroid_size; i++) {
            new_centroids[i] = 0.0;
        }

        #pragma omp target teams distribute parallel for reduction(max:changed)
        for (int i = 0; i < num_points; i++) {
            double min_dist = DBL_MAX;
            int best_cluster = 0;

            for (int j = 0; j < k; j++) {
                double dist = 0.0;
                for (int f = 0; f < num_features; f++) {
                    double diff = dataset[i * num_features + f] - centroids[j * num_features + f];
                    dist += diff * diff;
                }
                if (dist < min_dist) {
                    min_dist = dist;
                    best_cluster = j;
                }
            }

            if (labels[i] != best_cluster) {
                labels[i] = best_cluster;
                changed = 1;
            }

            #pragma omp atomic update
            cluster_sizes[best_cluster]++;

            for (int f = 0; f < num_features; f++) {
                #pragma omp atomic update
                new_centroids[best_cluster * num_features + f] += dataset[i * num_features + f];
            }
        }

        #pragma omp target teams distribute parallel for
        for (int i = 0; i < centroid_size; i++) {
            int cluster = i / num_features;
            if (cluster_sizes[cluster] > 0) {
                centroids[i] = new_centroids[i] / cluster_sizes[cluster];
            }
        }

        iterations++;
    }

    t0 = omp_get_wtime();
    #pragma omp target exit data map(delete: dataset[0:data_size], cluster_sizes[0:k], new_centroids[0:centroid_size]) \
                                 map(from: centroids[0:centroid_size], labels[0:num_points])
    *transfer_time += omp_get_wtime() - t0;

    free(cluster_sizes);
    free(new_centroids);
    return iterations;
}

int main() {
    double *dataset = NULL;
    double *centroids;
    int *labels;
    int num_points = 0;
    int iterations;
    double start;
    double end;
    double transfer_time;
    const char *output_file = "../../data/processed/results_openmp_gpu.csv";

    if (load_dataset(CLEAN_DATA_PATH, &dataset, &num_points) != 0) {
        return 1;
    }

    printf("Carregou %d pontos com %d features.\n", num_points, NUM_FEATURES);
    printf("Lendo as seguintes colunas: ");
    for (int i = 0; i < NUM_FEATURES; i++) {
        printf("%s%s", FEATURE_NAMES[i], (i == NUM_FEATURES - 1) ? "\n" : ", ");
    }

    centroids = (double *)malloc(NUM_CLUSTERS * NUM_FEATURES * sizeof(double));
    labels = (int *)malloc(num_points * sizeof(int));
    if (!centroids || !labels) {
        free(centroids);
        free(labels);
        free(dataset);
        return 1;
    }

    for (int i = 0; i < num_points; i++) {
        labels[i] = -1;
    }
    initialize_centroids(dataset, num_points, NUM_CLUSTERS, NUM_FEATURES, centroids);

    printf("Iniciando K-Means OpenMP GPU\n");
    start = omp_get_wtime();
    iterations = run_kmeans_openmp_gpu(dataset, num_points, NUM_CLUSTERS, NUM_FEATURES,
                                       100, centroids, labels, &transfer_time);
    end = omp_get_wtime();

    if (iterations < 0) {
        free(centroids);
        free(labels);
        free(dataset);
        return 1;
    }

    printf("K-means concluido com sucesso em %d interacoes\n", iterations);
    printf("Wall Clock Time: %.9f segundos\n", end - start);
    printf("Tempo de transferencia CPU-GPU: %.9f segundos\n", transfer_time);

    if (save_results(output_file, labels, num_points) == 0) {
        printf("Clusters salvo: %s\n", output_file);
    }

    free(centroids);
    free(labels);
    free(dataset);
    return 0;
}
