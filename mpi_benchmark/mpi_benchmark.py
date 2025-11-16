#!/usr/bin/env python3

from pathlib import Path
from math import ceil
import subprocess
import argparse
import statistics
from typing import Optional

import matplotlib.pyplot as plt

# Theoretical average time to send/receive one message (ex. MPI_Bcast)
COM_TIME = 0.000001

# Path to compilation target directory
BIN_DIR: Path = Path(__file__).parent.parent / "bin"

# Path to directory to store benchmark plots
PLOT_DIR: Path = Path(__file__).parent.parent / "plots"


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
    # Define and display execution command
    command: list[str] = ['mpiexec', '--oversubscribe', '-np', f'{num_processes}', str(exe_path), *args]
    print(f'Executing mpicc benchmark with {command}')
    
    # Execute program as subprocess
    result = subprocess.run(
        command,
        capture_output=True,
        text=True
    )

    # Display execution output
    print(f'Results:\n{result.stdout}\n{result.stderr}\n')

    # Return last word from program execution stdout as wall-clock benchmark time
    return float(result.stdout.split(' ')[-1])


def basic_pred_runtime(processes: list[int], base_time: float) -> list[float]:
    return [base_time / p for p in processes]


def run_benchmark(src_path: Path, processes: list[int], prgm_args: list[str], mpicc_args) -> tuple[list[float], list[float]]:
    # Define path to executable target
    target_path: Path = BIN_DIR / src_path.stem

    # Compile Program
    if not mpicc_compile(src_path, target_path, mpicc_args):
        return ([], []) # Compilation failed
    
    avg: list[float] = [] # Holds result of each run with p for averaging

    for p in processes: # Return list of benchmark results for each process amount
        # Benchmark average runtime
        runtimes: list[float] = [mpicc_benchmark(target_path, p, prgm_args) for _ in range(5)]
        avg.append(statistics.mean(runtimes)) # Take average of 5 runs

    return (avg, basic_pred_runtime(processes, avg[0]))


def plot_benchmark(
    processes: list[int], 
    exec_times: list[float], 
    prgm_name: str, 
    theoretical_times: Optional[list[float]] = None
) -> None:
    plot_path: Path = PLOT_DIR / f'{prgm_name}_benchmark_results.png'
    plt.plot(processes, exec_times, marker='o', label='Measured Runtime')

    if theoretical_times:
        plt.plot(processes, theoretical_times, marker='x', label='Theoretical Runtime')

    plt.xlabel('Average Execution Time (s)')
    plt.ylabel('Number of Processes')

    plt.title(f'MPI Benchmark of {prgm_name}')
    plt.grid(True)
    plt.savefig(plot_path, dpi=300, bbox_inches='tight')


def predict_runtime(p: int, m: int, n: int, x: float) -> float:
    return m * (x * ((n - 2 + p) // p) + 2 * COM_TIME)

def theoretical_runtimes(processes: list[int], m: int, n: int, x: float) -> list[float]:
    return [predict_runtime(p, m, n, x) for p in processes]

        
def main():
    # Create parse for program arguments
    parser: argeparse.ArgumentParser = argparse.ArgumentParser(
        description="CLI for Benchmarking OpenMPI Programs"
    )

    # Define program arguments
    parser.add_argument('prgm_path', help='Path to MPI source code')
    parser.add_argument('prgm_flags', nargs='*', help='Command line arguments for MPI program')

    parser.add_argument( # Compilation flags for mpicc
        '-cf', '--mpicc-flags', nargs='*', default=['O3', 'Wall', 'Wextra'], 
        help='Compilation flags for mpicc (default: -O3, -Wall, -Wextra)'
    )

    parser.add_argument( # Number of processes to benchmark
        '-p', '--processes',
        nargs='*',
        type=int,
        default=list(range(1, 11)),
        help='Number of processes to benchmark (default: 1-10)'
    )

    parser.add_argument(
        '-t', '--theoretical',
        action='store_true',
        help='Include theoretical runtimes in the benchmark plot'
    )

    args: argparse.Namespace = parser.parse_args() # Parse arguments

    # Extract command line args as variables
    prgm_path: Path = Path(args.prgm_path)
    prgm_flags: list[str] = args.prgm_flags
    mpicc_flags: list[str] | None = args.mpicc_flags
    processes: list[int] | None = args.processes
    include_theoretical: bool = args.theoretical

    # Get benchmark results
    benchmark_results, pred_results = run_benchmark(prgm_path, processes, prgm_flags, mpicc_flags)

    # Print results
    print(f"Processes:\t\t{processes}\nResults:\t\t{benchmark_results}\nPredicted Results:\t\t{pred_results}")


    if include_theoretical:
        plot_benchmark(processes, benchmark_results, prgm_path.name, pred_results)
    else:
        plot_benchmark(processes, benchmark_results, prgm_path.name)

if __name__ == "__main__":
    main()
