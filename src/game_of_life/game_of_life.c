#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "mpi.h"


#define BOARD_WIDTH 5
#define BOARD_HEIGHT 5


typedef struct {
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
} SubSquare;

static inline void factor(int n, int* x, int* y) {
    *y = (int) sqrt((double) n); // Set y as floor of square root of n

    while(n % *y && y > 1) // Decrement y to divide n
        (*y)--;
    
    *x = (n + *y - 1) / *y; // Set x as ceiling of n / y
}


void subSquareBounds(int width, int height, int n, int i, SubSquare* sqr) {
    // Holds sub grid size
    int rowWidth;
    int colWidth;

    factor(n, &rowWidth, &colWidth); // Determine size of sub grid


}



int main(int argc, char* argv[]) {
    int id, p; // Process id and number of MPI processes
    double elapsedTime; // Tracks wall-clock execution time

    // Read number of turns from args
    int turns = 10;

    // Initialize board
    const bool board[][BOARD_WIDTH] = {{0, 0, 1, 0, 1}, {1, 0, 1, 1, 0}, {1, 1, 0, 0, 1}, {0, 1, 1, 0, 1}, {0, 0, 1, 1, 1}};
    const size_t boardSize = BOARD_HEIGHT;
    const size_t boardColSize = BOARD_WIDTH;

    MPI_Init(&argc, &argv); // Initialize MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &id); // Assign process identifier to id
    MPI_Comm_size(MPI_COMM_WORLD, &p); // Assign number of processes to p

    MPI_Barrier(MPI_COMM_WORLD); // Wait for processes
    elapsedTime = -MPI_Wtime(); // Start benchmark time
    
    // Calculate start row
    // Calculate end row

    // Calculate sub board width
    // Calculate sub board height

    // Calculate enum neighbor cells

    // Calculat eneighbors

    // Create pointer to sub board
    // Create board of cells neighbors

    // Iterate through turns
    for(int i = 0; i < turns; i++) {
        // Iterate through sub board positions

            // if alive
                // Alert neighbors:
                
                // if out of process sub board
                    // Send asynchronous message
                // else update cell;s neighbor count
        
        // Send global 'done' message

        int procsFinishedSending = 0;
        bool canProceed = 0;

        while(!canProceed) {
            if
        }

        // Loop through message until buffer empty and done msg rcvd == p
            // update cells neighbor count
        
        // Apply updates to sub board
    }

    // deallocate neighbor count memory




    // Clean up MPI environment

    if(!id) {
        printf("Results:");
    }

    return 0;   
}