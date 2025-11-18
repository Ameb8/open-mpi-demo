#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "mpi.h"


#define MAX_COMBINATIONS 1000000
#define DIVISOR 100000
#define MIN_ID 100000

#define NUM_ID_MSG "\nThere are %d valid 6-digit combinations"
#define BENCHMARK_MSG "\nComputation time with %d processes (seconds): %10.6f\n"


// Determines if individual six-digit combination is valid ID
// Returns 1 if ID is valid, otherwise zero
bool validateId(int id) {
    int divisor = DIVISOR; // USed to divide ID to isolate digits
    int digitSum = 0; // Tracks sum of ID digits
    int lastDigit = -1;


    while(divisor > 0) { // Iterate through ID digits
        int digit = id / divisor; // Calculate next digit
        id %= divisor; // Remove digit from id
        divisor /= 10; // Reduce divisor

        if(digit == lastDigit) // Check if adjacent digits match
            return false;

        digitSum += digit; // Update digit sum
        lastDigit = digit; // Update last digit
    }

    // Check for disallowed digit sums
    if(digitSum == 7 || digitSum == 11 || digitSum == 13)
        return false;

    return true;
}


int main(int argc, char* argv[]) {
    int id, p, totalValidIds; // Process id, number of MPI processes, and number of valid IDs
    double elapsedTime; // Tracks wall-clock execution time
    int processValidIds = 0; // Tracks valid IDs found per process

    MPI_Init(&argc, &argv); // Initialize MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &id); // Assign process identifier to id
    MPI_Comm_size(MPI_COMM_WORLD, &p); // Assign number of processes to p

    MPI_Barrier(MPI_COMM_WORLD); // Wait for processes
    elapsedTime = -MPI_Wtime(); // Start benchmark time

    // Iterate through all combinations divisible by process rank
    for(int i = id + MIN_ID; i < MAX_COMBINATIONS; i += p)
        processValidIds += validateId(i);

    // Accumulate total valid IDs across processes
    MPI_Reduce(&processValidIds, &totalValidIds, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD); // Wait for all processes to complete

    elapsedTime += MPI_Wtime(); // End benchmark time

    if(!id) { // Display results from root process
        printf(NUM_ID_MSG, totalValidIds);// Display number of valid IDs
        printf(BENCHMARK_MSG, p, elapsedTime); // Display benchmark results
    }

    MPI_Finalize(); // Clean up MPI environment

    return 0;
}