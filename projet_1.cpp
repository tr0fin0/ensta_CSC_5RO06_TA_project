#include <iostream>
#include <Eigen/Dense>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace Eigen;

typedef Matrix<int, Dynamic, Dynamic> IntMatrix;

// Algorithme 1 : Multiplication naive
IntMatrix naiveMultiplication(const IntMatrix& A, const IntMatrix& B) {
    int n = A.rows();
    IntMatrix C(n, n);
    C.setZero(); // Initialize matrix C with zeros
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                C(i, j) += A(i, k) * B(k, j);
    return C;
}


// Algorithme 2 : Multiplication naive avec boucle réordonnées
IntMatrix naiveReorderedMultiplication(const IntMatrix& A, const IntMatrix& B) {
    int n = A.rows();
    IntMatrix C(n, n);
    C.setZero(); // Initialize matrix C with zeros
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k)
            for (int j = 0; j < n; ++j)
                C(i, j) += A(i, k) * B(k, j);
    return C;
}

// Algorithme 3 : Multiplication par blocs
IntMatrix blockMultiplication(const IntMatrix& A, const IntMatrix& B, int blockSize) {
    int n = A.rows();
    IntMatrix C(n, n);
    C.setZero(); // Initialize matrix C with zeros
    for (int ii = 0; ii < n; ii += blockSize)
        for (int jj = 0; jj < n; jj += blockSize)
            for (int kk = 0; kk < n; kk += blockSize)
                for (int i = ii; i < min(ii + blockSize, n); ++i)
                    for (int j = jj; j < min(jj + blockSize, n); ++j)
                        for (int k = kk; k < min(kk + blockSize, n); ++k)
                            C(i, j) += A(i, k) * B(k, j);
    return C;
}


// Algorithme 5 : Multiplication avec la bibliothèque Eigen
MatrixXd eigenMultiplication(const MatrixXd& A, const MatrixXd& B) {
    return A * B;
}


// Función para sumar matrices de tipo IntMatrix
IntMatrix addMatrix(const IntMatrix& A, const IntMatrix& B) {
    if (A.rows() != B.rows() || A.cols() != B.cols()) {
        throw std::invalid_argument("Matrix sizes do not match.");
    }
    return A + B;
}

// Función para restar matrices de tipo IntMatrix
IntMatrix subtractMatrix(const IntMatrix& A, const IntMatrix& B) {
    if (A.rows() != B.rows() || A.cols() != B.cols()) {
        throw std::invalid_argument("Matrix sizes do not match.");
    }
    return A - B;
}



// Strassen para combinar matrices
void multiplyMatrixStrassen(const IntMatrix& A, const IntMatrix& B, IntMatrix& C, int n) {
    if (n == 1) {
        C(0, 0) = A(0, 0) * B(0, 0);
        return;
    }

    int newSize = n / 2;
    IntMatrix A11 = A.topLeftCorner(newSize, newSize);
    IntMatrix A12 = A.topRightCorner(newSize, newSize);
    IntMatrix A21 = A.bottomLeftCorner(newSize, newSize);
    IntMatrix A22 = A.bottomRightCorner(newSize, newSize);

    IntMatrix B11 = B.topLeftCorner(newSize, newSize);
    IntMatrix B12 = B.topRightCorner(newSize, newSize);
    IntMatrix B21 = B.bottomLeftCorner(newSize, newSize);
    IntMatrix B22 = B.bottomRightCorner(newSize, newSize);

    IntMatrix C11(newSize, newSize);
    IntMatrix C12(newSize, newSize);
    IntMatrix C21(newSize, newSize);
    IntMatrix C22(newSize, newSize);

    IntMatrix M1(newSize, newSize);
    IntMatrix M2(newSize, newSize);
    IntMatrix M3(newSize, newSize);
    IntMatrix M4(newSize, newSize);
    IntMatrix M5(newSize, newSize);
    IntMatrix M6(newSize, newSize);
    IntMatrix M7(newSize, newSize);

    IntMatrix AResult(newSize, newSize);
    IntMatrix BResult(newSize, newSize);

    // M1 = (A11 + A22) * (B11 + B22)
    AResult = addMatrix(A11, A22);
    BResult = addMatrix(B11, B22);
    multiplyMatrixStrassen(AResult, BResult, M1, newSize);

    // M2 = (A21 + A22) * B11
    AResult = addMatrix(A21, A22);
    multiplyMatrixStrassen(AResult, B11, M2, newSize);

    // M3 = A11 * (B12 - B22)
    BResult = subtractMatrix(B12, B22);
    multiplyMatrixStrassen(A11, BResult, M3, newSize);

    // M4 = A22 * (B21 - B11)
    BResult = subtractMatrix(B21, B11);
    multiplyMatrixStrassen(A22, BResult, M4, newSize);

    // M5 = (A11 + A12) * B22
    AResult = addMatrix(A11, A12);
    multiplyMatrixStrassen(AResult, B22, M5, newSize);

    // M6 = (A21 - A11) * (B11 + B12)
    AResult = subtractMatrix(A21, A11);
    BResult = addMatrix(B11, B12);
    multiplyMatrixStrassen(AResult, BResult, M6, newSize);

    // M7 = (A12 - A22) * (B21 + B22)
    AResult = subtractMatrix(A12, A22);
    BResult = addMatrix(B21, B22);
    multiplyMatrixStrassen(AResult, BResult, M7, newSize);

    // C11 = M1 + M4 - M5 + M7
    C11 = addMatrix(addMatrix(M1, M4), subtractMatrix(M7, M5));

    // C12 = M3 + M5
    C12 = addMatrix(M3, M5);

    // C21 = M2 + M4
    C21 = addMatrix(M2, M4);

    // C22 = M1 - M2 + M3 + M6
    C22 = addMatrix(subtractMatrix(M1, M2), addMatrix(M3, M6));

    // Combinar submatrices en C
    C.topLeftCorner(newSize, newSize) = C11;
    C.topRightCorner(newSize, newSize) = C12;
    C.bottomLeftCorner(newSize, newSize) = C21;
    C.bottomRightCorner(newSize, newSize) = C22;
}



// Algoritmo BCRL para multiplicación de matrices
void multiplyMatrixBCRL(const MatrixXi& A, const MatrixXi& B, MatrixXi& C) {
    int n = A.rows();
    if (n == 1) {
        C(0, 0) = A(0, 0) * B(0, 0);
        return;
    }

    int newSize = n / 2;
    MatrixXi A11 = A.topLeftCorner(newSize, newSize);
    MatrixXi A12 = A.topRightCorner(newSize, newSize);
    MatrixXi A21 = A.bottomLeftCorner(newSize, newSize);
    MatrixXi A22 = A.bottomRightCorner(newSize, newSize);

    MatrixXi B11 = B.topLeftCorner(newSize, newSize);
    MatrixXi B12 = B.topRightCorner(newSize, newSize);
    MatrixXi B21 = B.bottomLeftCorner(newSize, newSize);
    MatrixXi B22 = B.bottomRightCorner(newSize, newSize);

    MatrixXi P1(newSize, newSize);
    MatrixXi P2(newSize, newSize);
    MatrixXi P3(newSize, newSize);
    MatrixXi P4(newSize, newSize);
    MatrixXi P5(newSize, newSize);
    MatrixXi P6(newSize, newSize);
    MatrixXi P7(newSize, newSize);

    // P1 = A11 * (B12 - B22)
    MatrixXi B12minusB22 = B12 - B22;
    multiplyMatrixBCRL(A11, B12minusB22, P1);

    // P2 = (A11 + A12) * B22
    MatrixXi A11plusA12 = A11 + A12;
    multiplyMatrixBCRL(A11plusA12, B22, P2);

    // P3 = (A21 + A22) * B11
    MatrixXi A21plusA22 = A21 + A22;
    multiplyMatrixBCRL(A21plusA22, B11, P3);

    // P4 = A22 * (B21 - B11)
    MatrixXi B21minusB11 = B21 - B11;
    multiplyMatrixBCRL(A22, B21minusB11, P4);

    // P5 = (A11 + A22) * (B11 + B22)
    MatrixXi A11plusA22 = A11 + A22;
    MatrixXi B11plusB22 = B11 + B22;
    multiplyMatrixBCRL(A11plusA22, B11plusB22, P5);

    // P6 = (A12 - A22) * (B21 + B22)
    MatrixXi A12minusA22 = A12 - A22;
    MatrixXi B21plusB22 = B21 + B22;
    multiplyMatrixBCRL(A12minusA22, B21plusB22, P6);

    // P7 = (A11 - A21) * (B11 + B12)
    MatrixXi A11minusA21 = A11 - A21;
    MatrixXi B11plusB12 = B11 + B12;
    multiplyMatrixBCRL(A11minusA21, B11plusB12, P7);

    // C11 = P5 + P4 - P2 + P6
    MatrixXi C11 = P5 + P4 - P2 + P6;

    // C12 = P1 + P2
    MatrixXi C12 = P1 + P2;

    // C21 = P3 + P4
    MatrixXi C21 = P3 + P4;

    // C22 = P5 + P1 - P3 - P7
    MatrixXi C22 = P5 + P1 - P3 - P7;

    // Combinar submatrices en C
    C.topLeftCorner(newSize, newSize) = C11;
    C.topRightCorner(newSize, newSize) = C12;
    C.bottomLeftCorner(newSize, newSize) = C21;
    C.bottomRightCorner(newSize, newSize) = C22;
}

void multiplyMatrixPan(const MatrixXi& A, const MatrixXi& B, MatrixXi& C) {
    int n = A.rows();
    
    if (n == 1) {
        C(0, 0) = A(0, 0) * B(0, 0);
        return;
    }

    int newSize = n / 2;
    MatrixXi A11 = A.topLeftCorner(newSize, newSize);
    MatrixXi A12 = A.topRightCorner(newSize, newSize);
    MatrixXi A21 = A.bottomLeftCorner(newSize, newSize);
    MatrixXi A22 = A.bottomRightCorner(newSize, newSize);

    MatrixXi B11 = B.topLeftCorner(newSize, newSize);
    MatrixXi B12 = B.topRightCorner(newSize, newSize);
    MatrixXi B21 = B.bottomLeftCorner(newSize, newSize);
    MatrixXi B22 = B.bottomRightCorner(newSize, newSize);

    MatrixXi P1(newSize, newSize);
    MatrixXi P2(newSize, newSize);
    MatrixXi P3(newSize, newSize);
    MatrixXi P4(newSize, newSize);
    MatrixXi P5(newSize, newSize);
    MatrixXi P6(newSize, newSize);
    MatrixXi P7(newSize, newSize);

    MatrixXi temp1(newSize, newSize);
    MatrixXi temp2(newSize, newSize);

    // P1 = (A11 + A22) * (B11 + B22)
    temp1 = A11 + A22;
    temp2 = B11 + B22;
    multiplyMatrixPan(temp1, temp2, P1);

    // P2 = (A21 + A22) * B11
    temp1 = A21 + A22;
    multiplyMatrixPan(temp1, B11, P2);

    // P3 = A11 * (B12 - B22)
    temp1 = B12 - B22;
    multiplyMatrixPan(A11, temp1, P3);

    // P4 = A22 * (B21 - B11)
    temp1 = B21 - B11;
    multiplyMatrixPan(A22, temp1, P4);

    // P5 = (A11 + A12) * B22
    temp1 = A11 + A12;
    multiplyMatrixPan(temp1, B22, P5);

    // P6 = (A21 - A11) * (B11 + B12)
    temp1 = A21 - A11;
    temp2 = B11 + B12;
    multiplyMatrixPan(temp1, temp2, P6);

    // P7 = (A12 - A22) * (B21 + B22)
    temp1 = A12 - A22;
    temp2 = B21 + B22;
    multiplyMatrixPan(temp1, temp2, P7);

    // C11 = P1 + P4 - P5 + P7
    MatrixXi C11 = P1 + P4 - P5 + P7;

    // C12 = P3 + P5
    MatrixXi C12 = P3 + P5;

    // C21 = P2 + P4
    MatrixXi C21 = P2 + P4;

    // C22 = P1 + P3 - P2 + P6
    MatrixXi C22 = P1 + P3 - P2 + P6;

    // Combinar submatrices en C
    C.topLeftCorner(newSize, newSize) = C11;
    C.topRightCorner(newSize, newSize) = C12;
    C.bottomLeftCorner(newSize, newSize) = C21;
    C.bottomRightCorner(newSize, newSize) = C22;
}

void printMatrix(const IntMatrix& matrix) {
    for (int i = 0; i < matrix.rows(); ++i) {
        for (int j = 0; j < matrix.cols(); ++j) {
            cout << matrix(i, j) << " ";
        }
        cout << endl;
    }
}


int main() {
    int n = 8; //Para Strassen hay que usar matrices de tamaño 2^n
    srand(time(0)); // Initialize the seed for random numbers

    // Generate random numbers between 1 and 10
    IntMatrix A = IntMatrix::NullaryExpr(n, n, []() { return rand() % 10 + 1; });
    IntMatrix B = IntMatrix::NullaryExpr(n, n, []() { return rand() % 10 + 1; });
    IntMatrix C5(n, n);
    IntMatrix C6(n, n);
    IntMatrix C7(n, n);

    // Naive multiplication
    IntMatrix C1 = naiveMultiplication(A, B);
    IntMatrix C2 = naiveReorderedMultiplication(A, B);
    IntMatrix C3 = blockMultiplication(A, B, 2);

    MatrixXd A_double = A.cast<double>();
    MatrixXd B_double = B.cast<double>();
    MatrixXd C4 = eigenMultiplication(A_double, B_double);

    // Strassen multiplication
    multiplyMatrixStrassen(A, B, C5, n);
    multiplyMatrixBCRL(A, B, C6);
    multiplyMatrixPan(A, B, C7);

    cout << "A:" << endl;
    printMatrix(A);
    cout << "B:" << endl;
    printMatrix(B);
    
    cout << "Resultado de la multiplicacion naive:" << endl;
    printMatrix(C1);
    cout << "Resultado de la multiplicacion naive reordenada:" << endl;
    printMatrix(C2);
    cout << "Resultado de la multiplicacion division por bloques:" << endl;
    printMatrix(C3);
    cout << "Resultado de la multiplicacion con Eigen:" << endl;
    cout << C4 << endl;
    cout << "Resultado de la multiplicacion con Strassen:" << endl;
    printMatrix(C5);
    cout << "Resultado de la multiplicacion con BCRL:" << endl;
    printMatrix(C6);
    cout << "Resultado de la multiplicacion con PAN:" << endl;
    printMatrix(C7);

    return 0;
}