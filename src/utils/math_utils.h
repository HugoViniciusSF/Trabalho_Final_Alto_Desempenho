#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// Distância Euclidiana entre dois pontos.
double euclidean_distance(const double *p1, const double *p2, int num_features);

// Inicializa os centroides escolhendo K pontos aleatórios únicos do dataset
void initialize_centroids(const double *dataset, int num_points, int k, int num_features, double *centroids);

// Pipeline completa do K-means
void run_kmeans(const double *dataset, int num_points, int k, int num_features, int max_iterations, double *centroids, int *labels);

#endif
