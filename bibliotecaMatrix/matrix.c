#include "matrix.h"

#define PRETTY_PRINT

void ZerarNatrix(Matrix *matrix)
{
    for (int i = 0; i < matrix->rows; ++i) {
        for (int j = 0; j < matrix->cols; ++j) {
            matrix->data[i][j] = 0;
        }
    }
}

void MatrixFree(Matrix **matrix)
{
    free((*matrix)->data[0]);
    free((*matrix)->data);
    free(*matrix);

    *matrix = NULL;
}

Matrix* GerarMatrix( int rows, int cols)
{
    Matrix* matrix = NULL;

    if (!(matrix = (Matrix *)malloc(sizeof(Matrix))))
    {
        return NULL;
    }

    matrix->rows = rows;
    matrix->cols = cols;

    /*
        O código a seguir é referente à alocação contínua da matriz na memória
        O jeito que fica na memória é um vetor unidimencional enorme (o segundo malloc),
        e um segundo vetor (primeiro malloc) utlizado para mapear o inicio de cada linha (feito no for)
     */
    matrix->data = (int**)malloc(sizeof(int*) * rows);

    int *temp = (int*)malloc(sizeof(int) * rows * cols);

    for (int i = 0; i < rows; ++i) {
      matrix->data[i] = (i * cols + temp);
    }

    return matrix;
}

void MatrixPrint(Matrix *matrix)
{
    printf("\n");
    for (int i = 0; i < matrix->rows; ++i) {
        for (int j = 0; j < matrix->cols; ++j) {
            #ifdef PRETTY_PRINT
                pretty_print_matrix_value(matrix->data[i][j]);
            #else
                printf("%3i ", matrix->data[i][j]);
            #endif
        }
        printf("\n");
    }
    printf("\n");
}

void SetMatrix(Matrix *matrix, int valor)
{
    for (int i = 0; i < matrix->rows; ++i) {
        for (int j = 0; j < matrix->cols; ++j) {
            matrix->data[i][j] = valor;
        }
    }
}

int MatrixCompare(Matrix *matrix_a, Matrix *matrix_b)
{
    int cols = matrix_a->cols;
    int rows = matrix_a->rows;

    if (cols != matrix_b->cols || rows != matrix_b->rows) {
        return 0;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (matrix_a->data[i][j] != matrix_b->data[i][j]) {
                return 0;
            }
        }
    }

    return 1;
}

void pretty_print_matrix_value(int value)
{
    if (value == 0) {
        printf("\033[1;36m");
    } else if (value <= 3) {
        printf("\033[1;34m");
    } else if (value <= 5) {
        printf("\033[1;32m");
    } else if (value < 10) {
        printf("\033[1;33m");
    } else {
        printf("\033[1;31m");
    }
    printf("%4i\033[0m ", value);
}
