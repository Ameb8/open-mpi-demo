#!/usr/bin/env python3

from pathlib import Path
import subprocess
import argparse

# Path to compilation target directory
BIN_DIR: Path = Path(__file__).parent.parent / "bin"


def mpicc_compile(src_path: Path, target_path: Path, args: list[str]) -> bool:
    # Define compilation command
    command: list[str] = ['mpicc', str(src_path), '-o', str(target_path), *[f'-{arg}' for arg in args]]
    print(f'\nCompiling {src_path} with:\n{command}\n')

    # Compile source code with mpicc
    result = subprocess.run(
        command,
        capture_output=True,
        text=True
    )

    # Display compilation results
    print(f'Results:\n{result.stdout}\n{result.stderr}\n')

    return result.returncode == 0


def mpicc_benchmark(exe_path: Path, num_processes: int, args: list[str]) -> float:
    # Execute program as subprocess
    result = subprocess.run(
        ['mpirun', '-np', f'{num_processes}', str(exe_path), *args],
        capture_output=True,
        text=True
    )

    # Return last word from program execution stdout as wall-clock benchmark time
    return float(' '.split(result.stdout)[-1])


def run_benchmark(src_path: Path, processes: list[int], prgm_args: list[str], mpicc_args) -> list[float]:
    # Define path to executable target
    target_path: Path = BIN_DIR / str(src_path).split('.')[-2]

    # Compile Program
    if not mpicc_compile(src_path, target_path, mpicc_args):
        return [] # Compilation failed
    
    # Return list of bencchmark results for each process amount
    return [mpicc_benchmark(target_path, p, prgm_args) for p in processes]

        
def main():
    # Create parse for program arguments
    parser: argeparse.ArgumentParser = argparse.ArgumentParser(
        description="CLI for Benchmarking OpenMPI Programs"
    )

    # Define program arguments
    parser.add_argument('prgm_path', help='Path to MPI source code')
    parser.add_argument('prgm_flags', nargs='*', help='Command line arguments for MPI program')

    parser.add_argument( # Compialtion flags for mpicc
        '-cf', '--mpicc-flags', nargs='*', default=['O3', 'Wall', 'Wextra'], 
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

    benchmark_results: list[float] = run_benchmark(prgm_path, processes, prgm_flags, mpicc_flags)

    print(f"Processess:\t\t{processes}\nResults:\t\t{benchmark_results}")

if __name__ == "__main__":
    main()
