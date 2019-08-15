#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "matrix.h"

Matrix *criar_matrix(int, int);

int main(const int argc, const char *argv[])
{
    if (argc != 3)
    {
        printf(
            "Erro! Numero incorreto de argumentos\n"
            "./mainMatrix COLS ROWS");
        return -1;
    }

    int cols = atoi(argv[1]);
    int rows = atoi(argv[2]);

    Matrix *matrix = criar_matrix(cols, rows);

    if (!matrix)
    {
        printf("Falha ao alocar matrix!\n");
        return -1;
    }

    printf("Matrix: %i, %i\n", matrix->cols, matrix->rows);

    return 0;
}

Matrix *criar_matrix(int cols, int rows)
{
    Matrix *matrix = NULL;

    if (!(matrix = (Matrix *)malloc(sizeof(Matrix))))
    {
        return NULL;
    }

    matrix->cols = cols;
    matrix->rows = rows;

    return matrix;
}
