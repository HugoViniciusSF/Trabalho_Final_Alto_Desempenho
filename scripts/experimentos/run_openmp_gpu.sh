#!/bin/bash
echo "Experimento OpenMP GPU"
cd src/openmp_gpu || exit

echo "Compilando kmeans_openmp_gpu"
make clean
make
echo "Executando"
./kmeans_openmp_gpu

cd ../..
echo "========================================"