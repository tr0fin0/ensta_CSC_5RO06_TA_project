#include "m_block.h"

// Algoritmo 3: Multiplicación por bloques
void blockMultiplication(const int A[SIZE][SIZE], const int B[SIZE][SIZE], int C[SIZE][SIZE], int blockSize) {
    for (int ii = 0; ii < SIZE; ii += blockSize)
        for (int jj = 0; jj < SIZE; jj += blockSize)
            for (int kk = 0; kk < SIZE; kk += blockSize)
                for (int i = ii; i < std::min(ii + blockSize, SIZE); ++i)
                    for (int j = jj; j < std::min(jj + blockSize, SIZE); ++j)
                        for (int k = kk; k < std::min(kk + blockSize, SIZE); ++k)
                            C[i][j] += A[i][k] * B[k][j];
}
