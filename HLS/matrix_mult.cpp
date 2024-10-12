#include "matrix_mult.h"

#ifdef solution_0_0
void matrix_mult_0_0(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream){
    #pragma HLS INTERFACE ap_ctrl_none port=return                 // disable unwanted messages
    #pragma HLS INTERFACE axis register both port=output_stream    //
    #pragma HLS INTERFACE axis register both port=input_stream     //
    // matrix multiplication function, A * B = C

    // matrix declaration
    matrix A[SIZE][SIZE];    // input
    matrix B[SIZE][SIZE];    // input
    matrix C[SIZE][SIZE];    // output

    // local variables
    int row, col, k;
    axis data_axis;


    // stream data read
    // matrix A
    loop_stream_read_row_A: for(row = 0; row < SIZE; row++){
        loop_stream_read_col_A: for(col = 0; col < SIZE; col++){
            A[row][col] = input_stream.read().data;
        }
    }
    // matrix B
    loop_stream_read_row_B: for(row = 0; row < SIZE; row++){
        loop_stream_read_col_B: for(col = 0; col < SIZE; col++){
            B[row][col] = input_stream.read().data;
        }
    }

    // naive matrix multiplication algorithm
    loop_mult_row: for(row = 0; row < SIZE; row++){
        loop_mult_col: for(col = 0; col < SIZE; col++){
            int sum = 0;    // same type as matrix

            loop_mult_k: for(k = 0; k < SIZE; k++){
                sum += A[row][k] * B[k][col];
            }
            C[row][col] = sum;
        }
    }

    //stream data write
    loop_stream_write_row: for(row = 0; row < SIZE; row++){
        loop_stream_write_col: for(col = 0; col < SIZE; col++){
            data_axis.data = C[row][col];

            if((row == SIZE-1) && (col == SIZE-1))
                data_axis.last = 1;
            else
                data_axis.last = 0;
            output_stream.write(data_axis);
        }
    }
}
#endif


#ifdef solution_0_1
void matrix_mult_0_1(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream){
    #pragma HLS INTERFACE ap_ctrl_none port=return                 // disable unwanted messages
    #pragma HLS INTERFACE axis register both port=output_stream    //
    #pragma HLS INTERFACE axis register both port=input_stream     //
    // matrix multiplication function, A * B = C

    // matrix declaration
    matrix A[SIZE][SIZE];    // input
    matrix B[SIZE][SIZE];    // input
    matrix C[SIZE][SIZE];    // output

    // local variables
    int row, col, k;
    axis data_axis;


    // stream data read
    // matrix A
    loop_stream_read_row_A: for(row = 0; row < SIZE; row++){
        loop_stream_read_col_A: for(col = 0; col < SIZE; col++){
            #pragma HLS PIPELINE    // optimization
            A[row][col] = input_stream.read().data;
        }
    }
    // matrix B
    loop_stream_read_row_B: for(row = 0; row < SIZE; row++){
        loop_stream_read_col_B: for(col = 0; col < SIZE; col++){
            #pragma HLS PIPELINE    // optimization
            B[row][col] = input_stream.read().data;
        }
    }

    // files were created with all algorithms inside to avoid function calls and reduce memory access therefore improving execution
    // naive matrix multiplication algorithm
    loop_mult_row: for(row = 0; row < SIZE; row++){
        loop_mult_col: for(col = 0; col < SIZE; col++){
            int sum = 0;    // same type as matrix

            loop_mult_k: for(k = 0; k < SIZE; k++){
                #pragma HLS PIPELINE    // optimization
                sum += A[row][k] * B[k][col];
            }
            C[row][col] = sum;
        }
    }

    //stream data write
    loop_stream_write_row: for(row = 0; row < SIZE; row++){
        loop_stream_write_col: for(col = 0; col < SIZE; col++){
            #pragma HLS PIPELINE    // optimization
            data_axis.data = C[row][col];

            if((row == SIZE-1) && (col == SIZE-1))
                data_axis.last = 1;
            else
                data_axis.last = 0;
            output_stream.write(data_axis);
        }
    }
}
#endif


#ifdef solution_0_2
void matrix_mult_0_2(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream){
    #pragma HLS INTERFACE ap_ctrl_none port=return                 // disable unwanted messages
    #pragma HLS INTERFACE axis register both port=output_stream    //
    #pragma HLS INTERFACE axis register both port=input_stream     //
    // matrix multiplication function, A * B = C

    // matrix declaration
    matrix A[SIZE][SIZE];    // input
    #pragma HLS ARRAY_PARTITION variable=A complete dim=2
    matrix B[SIZE][SIZE];    // input
    #pragma HLS ARRAY_PARTITION variable=B complete dim=1
    matrix C[SIZE][SIZE];    // output

    // local variables
    int row, col, k;
    axis data_axis;


    // stream data read
    // matrix A
    loop_stream_read_row_A: for(row = 0; row < SIZE; row++){
        loop_stream_read_col_A: for(col = 0; col < SIZE; col++){
            #pragma HLS PIPELINE    // optimization
            A[row][col] = input_stream.read().data;
        }
    }
    // matrix B
    loop_stream_read_row_B: for(row = 0; row < SIZE; row++){
        loop_stream_read_col_B: for(col = 0; col < SIZE; col++){
            #pragma HLS PIPELINE    // optimization
            B[row][col] = input_stream.read().data;
        }
    }


    // naive matrix multiplication algorithm
    loop_mult_row: for(row = 0; row < SIZE; row++){
        loop_mult_col: for(col = 0; col < SIZE; col++){
            #pragma HLS PIPELINE    // optimization

            int sum = 0;    // same type as matrix
            loop_mult_k: for(k = 0; k < SIZE; k++){
                sum += A[row][k] * B[k][col];
            }
            C[row][col] = sum;
        }
    }

    //stream data write
    loop_stream_write_row: for(row = 0; row < SIZE; row++){
        loop_stream_write_col: for(col = 0; col < SIZE; col++){
            #pragma HLS PIPELINE    // optimization
            data_axis.data = C[row][col];

            if((row == SIZE-1) && (col == SIZE-1))
                data_axis.last = 1;
            else
                data_axis.last = 0;
            output_stream.write(data_axis);
        }
    }
}
#endif


#ifdef solution_0_3
void matrix_mult_0_3(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream){
    #pragma HLS INTERFACE ap_ctrl_none port=return                 // disable unwanted messages
    #pragma HLS INTERFACE axis register both port=output_stream    //
    #pragma HLS INTERFACE axis register both port=input_stream     //
    // matrix multiplication function, A * B = C

    // matrix declaration
    matrix A[SIZE][SIZE];    // input
    #pragma HLS ARRAY_PARTITION variable=A complete dim=2
    matrix B[SIZE][SIZE];    // input
    #pragma HLS ARRAY_PARTITION variable=B complete dim=1
    matrix C[SIZE][SIZE];    // output

    // local variables
    int row, col, k;
    axis data_axis;


    // stream data read
    // matrix A
    loop_stream_read_row_A: for(row = 0; row < SIZE; row++){
        loop_stream_read_col_A: for(col = 0; col < SIZE; col++){
            #pragma HLS PIPELINE enable_flush rewind    // optimization
            A[row][col] = input_stream.read().data;
        }
    }
    // matrix B
    loop_stream_read_row_B: for(row = 0; row < SIZE; row++){
        loop_stream_read_col_B: for(col = 0; col < SIZE; col++){
            #pragma HLS PIPELINE enable_flush rewind    // optimization
            B[row][col] = input_stream.read().data;
        }
    }


    // naive matrix multiplication algorithm
    loop_mult_row: for(row = 0; row < SIZE; row++){
        loop_mult_col: for(col = 0; col < SIZE; col++){
            #pragma HLS PIPELINE enable_flush rewind    // optimization

            int sum = 0;    // same type as matrix
            loop_mult_k: for(k = 0; k < SIZE; k++){
                sum += A[row][k] * B[k][col];
            }
            C[row][col] = sum;
        }
    }

    //stream data write
    loop_stream_write_row: for(row = 0; row < SIZE; row++){
        loop_stream_write_col: for(col = 0; col < SIZE; col++){
            #pragma HLS PIPELINE enable_flush rewind    // optimization
            data_axis.data = C[row][col];

            if((row == SIZE-1) && (col == SIZE-1))
                data_axis.last = 1;
            else
                data_axis.last = 0;
            output_stream.write(data_axis);
        }
    }
}
#endif
