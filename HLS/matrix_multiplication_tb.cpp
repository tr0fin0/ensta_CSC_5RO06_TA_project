#include "matrix_multiplication_tb.h"

#include <stdio.h>
#include <math.h>
#include <cstdlib>

int main(){
    // matrix declaration
    int A[SIZE][SIZE];    // input
    int B[SIZE][SIZE];    // input
    int C0[SIZE][SIZE];  // output naive calculation
    int blockSize = BLOCK_SIZE;
    // local variables
    int row, col;
    // create A and B matrix
    loop_rand_row: for(row = 0; row < SIZE; row++){
        loop_rand_col: for(col = 0; col < SIZE; col++){
            A[row][col] = rand()%10;
            B[row][col] = rand()%10;
        }
    }

    // call benchmark function
	#ifdef solution_naive
	naiveMultiplication(A, B, C0);
	#endif

	#ifdef solution_naive_reordered
	naiveReorderedMultiplication(A, B, C0);
	#endif

	#ifdef solution_block
	blockMultiplication(A, B, C0, blockSize);
	#endif

	#ifdef solution_block_reordered
	blockReorderedMultiplication(A, B, C0, blockSize);
	#endif
}
