#include "matrix_mult_tb.h"
#include <stdio.h>
#include <math.h>
#include <cstdlib>


void matrix_mult_naive(int A[SIZE][SIZE], int B[SIZE][SIZE], int C[SIZE][SIZE]){
    int row, col, k;

    // naive matrix multiplication algorithm
    loop_mult_row: for(row = 0; row < SIZE; row++){
        loop_mult_col: for(col = 0; col < SIZE; col++){
            int sum = 0;
            loop_mult_k: for(k = 0; k < SIZE; k++){
                sum += A[row][k] * B[k][col];
            }
            C[row][col] = sum;
        }
    }
}


int main(){
    // matrix declaration
    int A[SIZE][SIZE];    // input
    int B[SIZE][SIZE];    // input
    int C0[SIZE][SIZE];    // output naive calculation
    int C[SIZE][SIZE];    // output

    // local variables
    int row, col, k;
    axis data_axis;

    // create A and B matrix
    loop_rand_row: for(row = 0; row < SIZE; row++){
        loop_rand_col: for(col = 0; col < SIZE; col++){
            A[row][col] = rand() % 10 + 1;    // same definition as in software
            B[row][col] = rand() % 10 + 1;
        }
    }

    // call benchmark function
    matrix_mult_naive(A, B, C0);

    //stream data write
    hls::stream<axis> input_stream, output_stream;

    loop_stream_write_A_row: for(row = 0; row < SIZE; row++){
        loop_stream_write_A_col: for(col = 0; col < SIZE; col++){
            data_axis.data = A[row][col];

            if((row == SIZE-1) && (col == SIZE-1))
                data_axis.last = 1;
            else
                data_axis.last = 0;
            input_stream.write(data_axis);
        }
    }
    loop_stream_write_B_row: for(row = 0; row < SIZE; row++){
        loop_stream_write_B_col: for(col = 0; col < SIZE; col++){
            data_axis.data = B[row][col];

            if((row == SIZE-1) && (col == SIZE-1))
                data_axis.last = 1;
            else
                data_axis.last = 0;
            input_stream.write(data_axis);
        }
    }

    // algorithm call, hardware execution
    #ifdef solution_0_0    // int naive algorithm without optimizations
        matrix_mult_0_0(input_stream, output_stream);
    #endif
    #ifdef solution_0_1    // int naive algorithm with pipeline
        matrix_mult_0_1(input_stream, output_stream);
    #endif
    #ifdef solution_0_2    // int naive algorithm with pipeline
        matrix_mult_0_2(input_stream, output_stream);
    #endif
    #ifdef solution_0_3    // int naive algorithm with pipeline
        matrix_mult_0_3(input_stream, output_stream);
    #endif



    // stream data read
    loop_stream_read_row: for(row = 0; row < SIZE; row++){
        loop_stream_read_col: for(col = 0; col < SIZE; col++){
            C[row][col] = output_stream.read().data;
        }
    }

    // results comparison
    loop_results_row: for(row = 0; row < SIZE; row++){
        loop_results_col: for(col = 0; col < SIZE; col++){
            if(fabs(C0[row][col] - C[row][col]) > 0.01){
                printf("error. C0[%d][%d] != C[%d][%d], with %f != %f.", row, col, row, col, C0[row][col], C[row][col]);
                return 1;
            }
        }
    }
}
