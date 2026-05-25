#ifndef IO_UTILS_H
#define IO_UTILS_H

// Lê o dataset de um arquivo CSV para um array 1D contíguo.
int load_dataset(const char *filename, double **dataset, int *num_points);

// Salva o index e cluster id em um arquivo csv
int save_results(const char *filename, int *labels, int num_points);

#endif
