#!/usr/bin/env bash

# Paths to source code
MODULE_NAME="mpi_benchmark.mpi_benchmark"
SRC_DIR="src"

# Default n for sieve
DEFAULT_SIEVE_ARG="1000000"

n="${1:-$DEFAULT_SIEVE_ARG}" # Set n to default or user param

# Execute benchmark
echo -e "\nRunning Sieve Benchmark with n=$n\n"
python3 -m "$MODULE_NAME" "$SRC_DIR/sieve/sieve.c" "$SRC_DIR/sieve/sieve_pipe.c" --args "$n" -cf O3 Wall Wextra lm 
