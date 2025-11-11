#!/usr/bin/env bash


MODULE_NAME="mpi_benchmark.mpi_benchmark"
SRC_DIR="src"

# Execute benchmark
echo -e "\nRunning Satisfiabilty problem SATI.c"
python3 -m "$MODULE_NAME" "$SRC_DIR/sati/SATI.c" "$n" -t
