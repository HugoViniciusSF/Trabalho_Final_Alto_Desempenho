CC = mpicc
CFLAGS = -Wall -O3 -fopenmp -DUSE_MPI

UTILS_DIR = ../utils
INCLUDES = -I$(UTILS_DIR)
SOURCES = main.c $(UTILS_DIR)/io_utils.c $(UTILS_DIR)/math_utils.c

all: kmeans_mpi_openmp

kmeans_mpi_openmp:
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o kmeans_mpi_openmp -lm

clean:
	rm -f kmeans_mpi_openmp
