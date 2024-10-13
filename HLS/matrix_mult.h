#include <stdio.h>
#include <hls_stream.h>
#include <ap_int.h>
//#include "reference.h"
#define BLOCK_SIZE 2

// define matrix data type
typedef int matrix;

// define axis data structure
struct axis{
    matrix data;
    ap_uint<1> last;
};

#ifdef solution_0_0
void matrix_mult_0_0(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream);
#endif

#ifdef solution_0_1
void matrix_mult_0_1(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream);
#endif

#ifdef solution_0_2
void matrix_mult_0_2(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream);
#endif

#ifdef solution_0_3
void matrix_mult_0_3(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream);
#endif
