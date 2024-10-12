#include "m_block_reordered.h"

// Algoritmo 4: Multiplicación por bloques reordenada
void blockReorderedMultiplication(const int A[SIZE][SIZE], const int B[SIZE][SIZE], int C[SIZE][SIZE], int blockSize) {
    for (int ii = 0; ii < SIZE; ii += blockSize)
        for (int kk = 0; kk < SIZE; kk += blockSize)
            for (int jj = 0; jj < SIZE; jj += blockSize)
                for (int i = ii; i < std::min(ii + blockSize, SIZE); ++i)
                    for (int k = kk; k < std::min(kk + blockSize, SIZE); ++k)
                        for (int j = jj; j < std::min(jj + blockSize, SIZE); ++j)
                            C[i][j] += A[i][k] * B[k][j];
}
