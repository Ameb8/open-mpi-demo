#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "mpi.h"


#define BOARD_WIDTH 5
#define BOARD_HEIGHT 5


typedef struct {
    // Top left corner
    int xStart;
    int yStart;
    
    // Dimensions
    int rows;
    int cols;
} SubSquare;


static inline void factor(int n, int* x, int* y) {
    *y = (int) sqrt((double) n); // Set y as floor of square root of n

    while(n % *y && y > 1) // Decrement y to divide n
        (*y)--;
    
    *x = (n + *y - 1) / *y; // Set x as ceiling of n / y
}


static inline int getTrueIndex(int subSize, int subBalance, int subIndex) {
    // Assume all lesser sub grids are padded
    int boardIndex = subIndex * (subSize + 1);

    if(subIndex > subBalance) // Account for non-padded sub grids
        boardIndex -= subIndex - subBalance;

    return boardIndex;
}


// Calculate location and size of sub grid for a given process
// Assigns results to fields of SubSquare struct argument
void subSquareBounds(int width, int height, int n, int i, SubSquare* sqr) {
    if(!sqr) return; // Validate inputs

    int numSubRows, numSubCols;

    factor(n, &numSubRows, &numSubCols); // Determine size of sub grid

    // Calculate base dimensions of sub grids
    sqr->cols = width / numSubRows;
    sqr->rows = height / numSubCols;

    // Calculate number of sub grids with base dimension + 1
    int bigWidth = width * n;
    int bigHeight = height % n;

    // Get index within grid of sub square
    int subRow = i % numSubRows;
    int subCol = i % numSubCols;

    // Calculate coordinate of sub grid top left
    sqr->xStart = getTrueIndex(sqr->rows, bigHeight, subRow);
    sqr->yStart = getTrueIndex(sqr->cols, bigWidth, subCol);

    // Increment sub grid size if padded
    if(subRow <= bigHeight) sqr->rows++;
    if(subCol <= bigWidth) sqr->cols++;
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

    // Calculate location and size of sub square to process
    SubSquare subSqr;
    subSquareBounds(BOARD_WIDTH, BOARD_HEIGHT, p, id, &subSqr);

    // Calculate number of neighbors
    int numNeighbors = 4;

    // Create buffer to hold updated cells
    int numUpdated = 0;
    //int updatedCells = malloc();
    
    // Iterate through turns
    for(int i = 0; i < turns; i++) {
        // Iterate through update cells
            // Update living neighbor count
    
        // Send done message to neighbors

        int doneNeighbors = 0;
        numUpdated = 0;

        // Read messages until all neighbors done and receive buffer empty
        //while(doneNeighbors < numNeighbors) {
            // Receive message

            // if message done
                doneNeighbors++;
            // else // Process neighbor subsquare update
                // Update living neighbor count
        //}

        
        // Iterate
    }




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




    if(!id) { // Clean up MPI environment
        printf("Results:");
    }

    return 0;   
}