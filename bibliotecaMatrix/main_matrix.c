#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "matrix.h"

int main(const int argc, const char *argv[])
{
    if (argc != 3)
    {
        printf(
            "Erro! Numero incorreto de argumentos\n"
            "./mainMatrix ROWS COLS"
        );
        return -1;
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    Matrix *matrix = GerarMatrix(rows, cols);

    if (!matrix) {
        printf("Erro ao alocar a matriz\n");
        return -2;
    }

    printf("Matrix: %i, %i\n", matrix->rows, matrix->cols);

    SetMatrix(matrix, 1);
    MatrixPrint(matrix);

    Matrix* matrix_b = GerarMatrix(rows, cols);
    ZerarNatrix(matrix_b);
    MatrixPrint(matrix_b);

    printf(MatrixCompare(matrix, matrix_b) ? "São iguais!\n" : "São diferentes!\n");

    SetMatrix(matrix_b, 10);
    MatrixPrint(matrix_b);

    printf(MatrixCompare(matrix, matrix_b) ? "São iguais!\n" : "São diferentes!\n");

    MatrixFree(&matrix);
    MatrixFree(&matrix_b);
    return 0;
}
