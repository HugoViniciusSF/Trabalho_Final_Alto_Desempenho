CC = gcc
# sm_80 e o maximo suportado pelo GCC 13; -fno-stack-protector necessario pois o PTX nao tem stack canary
CFLAGS = -Wall -O3 -fopenmp -fno-stack-protector -foffload=nvptx-none="-misa=sm_80 -O3 -lm -fcf-protection=none -fno-stack-protector"

UTILS_DIR = ../utils
INCLUDES = -I$(UTILS_DIR)
SOURCES = main.c $(UTILS_DIR)/io_utils.c $(UTILS_DIR)/math_utils.c

all: kmeans_openmp_gpu

kmeans_openmp_gpu:
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o kmeans_openmp_gpu -lm

clean:
	rm -f kmeans_openmp_gpu
