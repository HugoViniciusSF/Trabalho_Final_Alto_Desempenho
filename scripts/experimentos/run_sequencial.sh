#!/bin/bash
echo "Experimento Sequencial"
cd src/sequencial || exit

echo "Compilando kmeans_seq"
make clean
make
echo "Executando"
./kmeans_seq

cd ../..
echo "========================================"