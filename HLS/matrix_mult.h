#include <stdio.h>
#include <hls_stream.h>
#include <ap_int.h>

// define matrix data type
typedef int matrix;

// define axis data structure
struct axis{
    matrix data;
    ap_uint<1> last;
};

#ifdef solution0
void matrix_mult_0(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream);
#endif

#ifdef solution1
void matrix_mult_1(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream);
#endif

#ifdef solution2
void matrix_mult_2(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream);
#endif

#ifdef solution3
void matrix_mult_3(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream);
#endif
