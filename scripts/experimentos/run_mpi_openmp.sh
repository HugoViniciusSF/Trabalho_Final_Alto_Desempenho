#!/bin/bash
echo "Experimento MPI + OpenMP"
cd src/mpi_openmp || exit

echo "Compilando kmeans_mpi_openmp"
make clean
make
for procs in 1 2 4; do
    for threads in 1 2 4; do
        echo ""
        echo "Executando com $procs Processo(s) MPI e $threads Thread(s) OpenMP"
        OMP_NUM_THREADS=$threads mpirun -np $procs ./kmeans_mpi_openmp
    done
done

cd ../..
echo "========================================"