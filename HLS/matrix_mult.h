#include <stdio.h>
#include <hls_stream.h>
#include <ap_int.h>

#define SIZE 8        // matrix size
#define solution1     // active solution

// define matrix data type
#ifdef solution1
typedef float matrix;
#endif

// define axis data structure
struct axis{
    matrix data;
    ap_uint<1> last;
};

void matrix_mult_1(hls::stream<axis> &input_stream, hls::stream<axis> &output_stream);
