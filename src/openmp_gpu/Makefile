CC = gcc
CFLAGS = -Wall -O3 -fopenmp

UTILS_DIR = ../utils
INCLUDES = -I$(UTILS_DIR)
SOURCES = main.c $(UTILS_DIR)/io_utils.c $(UTILS_DIR)/math_utils.c

all: kmeans_openmp_gpu

kmeans_openmp_gpu:
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o kmeans_openmp_gpu -lm

clean:
	rm -f kmeans_openmp_gpu
