#include "../utils/dataset_config.h"
#include "../utils/io_utils.h"
#include "../utils/math_utils.h"
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    struct timespec start, end;
    double *dataset = NULL;
    int num_points = 0;

    // Process 0 loads the dataset
    if (rank == 0) {
        if (load_dataset(CLEAN_DATA_PATH, &dataset, &num_points) == 0) {
            printf("Carregou %d pontos com %d features.\n", num_points, NUM_FEATURES);
            printf("Lendo as seguintes colunas: ");
            for (int i = 0; i < NUM_FEATURES; i++) {
                printf("%s%s", FEATURE_NAMES[i], (i == NUM_FEATURES - 1) ? "\n" : ", ");
            }
            printf("Processos MPI: %d\n", size);

            // Just to print OpenMP threads info
            int num_threads = 1;
#pragma omp parallel
            {
#pragma omp single
                num_threads = omp_get_num_threads();
            }
            printf("Threads OpenMP por processo: %d\n", num_threads);
        } else {
            num_points = -1; // Indicate failure
        }
    }

    // Broadcast num_points to all processes
    MPI_Bcast(&num_points, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (num_points < 0) {
        MPI_Finalize();
        return 1;
    }

    // Calculate sendcounts and displacements for Scatterv and Gatherv
    int *sendcounts = (int *)malloc(size * sizeof(int));
    int *displs = (int *)malloc(size * sizeof(int));
    int *label_sendcounts = (int *)malloc(size * sizeof(int));
    int *label_displs = (int *)malloc(size * sizeof(int));

    int rem = num_points % size;
    int offset = 0;
    int label_offset = 0;
    for (int i = 0; i < size; i++) {
        int count = num_points / size + (i < rem ? 1 : 0);
        label_sendcounts[i] = count;
        label_displs[i] = label_offset;
        label_offset += count;

        sendcounts[i] = count * NUM_FEATURES;
        displs[i] = offset;
        offset += sendcounts[i];
    }

    int num_local_points = label_sendcounts[rank];
    double *local_dataset = (double *)malloc(sendcounts[rank] * sizeof(double));

    // Scatter the dataset to all processes
    MPI_Scatterv(dataset, sendcounts, displs, MPI_DOUBLE, local_dataset, sendcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double *centroids = (double *)malloc(NUM_CLUSTERS * NUM_FEATURES * sizeof(double));
    if (rank == 0) {
        initialize_centroids(dataset, num_points, NUM_CLUSTERS, NUM_FEATURES, centroids);
    }

    // Broadcast initial centroids to all processes
    MPI_Bcast(centroids, NUM_CLUSTERS * NUM_FEATURES, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int *local_labels = (int *)malloc(num_local_points * sizeof(int));
    for (int i = 0; i < num_local_points; i++) {
        local_labels[i] = -1;
    }

    // Barrier before timing to ensure all processes start at the same time
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        clock_gettime(CLOCK_MONOTONIC, &start);
    }

    // Run parallel K-Means
    run_kmeans_mpi_openmp(local_dataset, num_local_points, NUM_CLUSTERS, NUM_FEATURES, 100, centroids, local_labels, rank, size);

    // Barrier after timing
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        clock_gettime(CLOCK_MONOTONIC, &end);
    }

    int *global_labels = NULL;
    if (rank == 0) {
        global_labels = (int *)malloc(num_points * sizeof(int));
    }

    // Gather labels back to process 0
    MPI_Gatherv(local_labels, num_local_points, MPI_INT, global_labels, label_sendcounts, label_displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double tempo_exec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Wall Clock Time: %.9f segundos\n", tempo_exec);

        const char *output_file = "../../data/processed/results_mpi_openmp.csv";
        if (save_results(output_file, global_labels, num_points) == 0) {
            printf("Clusters salvo: %s\n", output_file);
        }

        free(global_labels);
        free(dataset);
    }

    free(sendcounts);
    free(displs);
    free(label_sendcounts);
    free(label_displs);
    free(local_dataset);
    free(centroids);
    free(local_labels);

    MPI_Finalize();
    return 0;
}
