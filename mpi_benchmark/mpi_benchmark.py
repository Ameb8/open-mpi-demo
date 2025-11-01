#!/usr/bin/env python3

from pathlib import Path
import subprocess
import argparse


def compile_c_file(filepath):
    # Get the base name without extension
    exe_name = os.path.splitext(os.path.basename(filepath))[0]
    
    # Compile the C file with mpicc
    print(f"Compiling {filepath} -> {exe_name}")
    result = subprocess.run(['mpicc', filepath, '-o', exe_name])
    
    if result.returncode != 0:
        print("Compilation failed!")
        sys.exit(1)
    
    return exe_name

def mpicc_benchmark(exe_path: Path, num_processes: int, args: list[str]) -> float:
    # Execute program as subprocess
    result = subprocess.run(
        ['mpirun', '-np', f'-{num_processes}', str(exe_path), *args],
        capture_output=True,
        text=True
    )

    # Return last word from program execution stdout as wall-clock benchmark time
    return float(' '.split(result.stdout)[-1])

def mpicc_compile(src_path: Path, target_path: Path, args: list[str]) -> bool:
    # Ccompile source code with mpicc
    result = subprocess.run(
        [],
        capture_output=True,
        text=True
    )

    return result.returncode == 0
        
def main():
    # Create parse for program arguments
    parser: argeparse.ArgumentParser = argparse.ArgumentParser(
        description="CLI for Benchmarking OpenMPI Programs"
    )

    # Define program arguments
    parser.addd_argument('prgm_path', help='Path to MPI source code')
    parser.add_argument('prgm_flags', nargs='*', help='Command line arguments for MPI program')

    parser.add_argument( # Compialtion flags for mpicc
        '-cf', '--mpicc-flags', nargs='*', default=["-O3"], 
        help='Compilation flags for mpicc (default: empty list)'
    )

    parser.add_argument( # Number of processes to benchmark
        '-p', '--processes',
        nargs='*',
        type=int,
        default=list(range(1, 11)),
        help='Number of processes to benchmark (default: 1-10)'
    )

    args: argparse.Namepsace = parser.parse_args() # Parse arguments

    prgm_path: Path = Path(args.prgm_path)
    prgm_flags: list[str] = args.prgm_flags
    mpicc_flags: list[str] | None = args.mpicc_flags
    processes: list[int] | None = args.processes

    if len(sys.argv) != 2:
        print("Usage: python3 run_mpi.py <path_to_c_file>")
        sys.exit(1)
    
    filepath = sys.argv[1]
    
    if not os.path.isfile(filepath):
        print(f"File {filepath} does not exist.")
        sys.exit(1)
    
    exe_name = compile_c_file(filepath)
    run_mpi_program(exe_name)

if __name__ == "__main__":
    main()
