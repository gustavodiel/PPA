#ifndef __MATRIX_C__
#define __MATRIX_C__

typedef struct matrix
{
    int **raw_matrix;
    int cols;
    int rows;
} Matrix;

int mzerar(Matrix *);
int mliberar(Matrix *);
int malocar(Matrix *);
int mimprimir(Matrix *);
int mgerar(Matrix *, int);
int mcomparar(Matrix *, Matrix *);

#endif // __MATRIX_C__