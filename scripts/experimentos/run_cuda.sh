#!/bin/bash
echo "Experimento CUDA"
cd src/cuda || exit

echo "Compilando kmeans_cuda"
make clean
make
echo "Executando"
./kmeans_cuda

cd ../..
echo "========================================"