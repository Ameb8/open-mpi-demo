#!/usr/bin/env bash


MODULE_NAME="mpi_benchmark.mpi_benchmark"
SRC_DIR="src"

# Execute benchmark
echo -e "\nDetermining number of valid 6-digit IDs\n"
python3 -m "$MODULE_NAME" "$SRC_DIR/valid_ids/unique_ids.c" --args "$n" -t
