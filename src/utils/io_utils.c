#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io_utils.h"
#include "dataset_config.h"

#define MAX_LINE_LENGTH 512
#define MAX_COLUMNS 100 // Número total máximo permitido de colunas no CSV

int load_dataset(const char *filename, double **dataset, int *num_points) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo CSV");
        return -1;
    }
    char line[MAX_LINE_LENGTH];
    int capacity = 400;
    int count = 0;
    *dataset = (double *)malloc(capacity * NUM_FEATURES * sizeof(double));
    if (!*dataset) {
        perror("Erro ao tentar alocar a memória");
        fclose(file);
        return -1;
    }
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        printf("Aviso: O arquivo está vazio.\n");
        fclose(file);
        return 0;
    }
    int keep_column[MAX_COLUMNS] = {0}; 
    int current_col = 0;
    int features_found = 0;
    char *p = line;

    // Identificar as colunas númericas definidas no arquivo de config do dataset
    // Comparo a string entre vírgulas
    while (*p && *p != '\n' && *p != '\r' && current_col < MAX_COLUMNS) {
        int len = strcspn(p, ",\r\n");
        char sep = p[len];
        p[len] = '\0';
        char *header_token = p;

        for (int k = 0; k < NUM_FEATURES; k++) {
            if (strcmp(header_token, FEATURE_NAMES[k]) == 0) {
                keep_column[current_col] = 1; 
                features_found++;
                break;
            }
        }
        p += len;
        if (sep == ',') p++;
        current_col++;
    }

    if (features_found != NUM_FEATURES) {
        printf("Nem todas as colunas de características estão presentes no arquivo.\n");
        printf("Colunas esperadas:\n");        
        for (int i = 0; i < NUM_FEATURES; i++) {
            printf(" -> %s\n", FEATURE_NAMES[i]);
        }
        free(*dataset);
        fclose(file);
        return -1;
    }

    // Leitura linha a linha
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        if (count >= capacity) {
            capacity *= 2;
            double *temp = (double *)realloc(*dataset, capacity * NUM_FEATURES * sizeof(double));
            if (!temp) {
                perror("Erro de realocação de memória");
                free(*dataset);
                fclose(file);
                return -1;
            }
            *dataset = temp;
        }
        char *p = line;
        int col_index = 0;      
        int feature_index = 0;  
        while (*p && *p != '\n' && *p != '\r' && col_index < MAX_COLUMNS) {
            int len = strcspn(p, ",\r\n");
            char sep = p[len];
            p[len] = '\0';
            char *token = p;
            if (keep_column[col_index] == 1) {
                (*dataset)[count * NUM_FEATURES + feature_index] = atof(token);
                feature_index++;
            }
            p += len;
            if (sep == ',') p++;
            col_index++;
        }
        count++;
    }
    *num_points = count;
    fclose(file);
    return 0;
}

int save_results(const char *filename, int *labels, int num_points) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao salvar resultados");
        return -1;
    }
    fprintf(file, "point_id,cluster_id\n");
    for (int i = 0; i < num_points; i++) {
        fprintf(file, "%d,%d\n", i, labels[i]);
    }
    fclose(file);
    return 0;
}
