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


void subSquarePrint(SubSquare* grid, int id) {
    printf("\n\nPROCESS %d:\tSTART: (%d, %d)\tSIZE: (%d x %d)\n", id, grid->xStart, grid->yStart, grid->rows, grid->cols);
}


static inline void factor(int n, int* x, int* y) {
    *y = (int) sqrt((double) n); // Set y as floor of square root of n

    while(n % *y && *y > 1) // Decrement y to divide n
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
    sqr->rows = height / numSubRows;
    sqr->cols = width / numSubCols;

    // Calculate number of sub grids with base dimension + 1
    int bigCols = width % numSubCols;
    int bigRows = height % numSubRows;

    // Get index within grid of sub square
    int subRow = i / numSubCols;
    int subCol = i % numSubCols;

    // Calculate coordinate of sub grid top left
    sqr->yStart = getTrueIndex(sqr->rows, bigRows, subRow);
    sqr->xStart = getTrueIndex(sqr->cols, bigCols, subCol);

    // Increment sub grid size if padded
    if(subRow < bigRows) sqr->rows++;
    if(subCol < bigCols) sqr->cols++;
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

    // DEBUG ***
    subSquarePrint(&subSqr, id);

    // Allocate new sub grid with 1 cell border
    // Initialize sub grid to full game board state
    // Allocate 'next' sub grid with 1 cell border (do not initialize)

    // Create subgrid int array (with border) to track cell counts

    // Iterate through turns
    for(int i = 0; i < turns; i++) {
        // Set all elements in enighborCount to zero

        // Post non-blocking receives for all border halos from neighbor

        // Send non-blocking local border rows/cols in subgrid to neighbor

        // update subgrid border rows when messages received

        // iterate through non-border cells of halo in sub grid
            // If cell is active
                // increment neighbor count locally only including border counts


        // iterate through sub grid cells not bordering halo
            // If cell is alive:
                // Dies if neighbor count < 2 or > 3
                // Lives if neighbor count == 2 or 3
            // If cell is dead:
                // Becomes alive if neighbor count == 3

            // Apply results to 'next' array

        // wait for all halo messages

        // Count neighboring cells of border
        // Update next subgrid for neighbor cells

        // Swap pointers for next and subGrid arrays
    
    }


    // deallocate neighbor count memory


    

    if(!id) { // Clean up MPI environment
        printf("Results:");
    }
    */

    MPI_Finalize(); // Clean up MPI environment

    return 0;   
}