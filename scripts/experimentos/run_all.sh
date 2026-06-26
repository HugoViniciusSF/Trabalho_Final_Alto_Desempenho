#!/bin/bash

echo "Todos os testes do K-Means"

./scripts/experimentos/run_sequencial.sh
./scripts/experimentos/run_mpi_openmp.sh
./scripts/experimentos/run_openmp_gpu.sh
./scripts/experimentos/run_cuda.sh

echo "Todos os experimentos concluídos"