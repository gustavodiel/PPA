#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct matrix
{
    int **data;
    int cols;
    int rows;
} Matrix;

/*
 * GerarMatrix
 *
 * Gera uma matriz de COLSxROWS
 *
 * Parametros:
 * cols: Inteiro
 * rows: Inteiro
 *
 * Retorno: Matrix* (NULL se falhar ao alocar)
 */
Matrix* GerarMatrix(int, int);


/*
 * ZerarMatrix
 *
 * Muda todos os valore da matriz para 0
 *
 * Parametros:
 * Matrix*
 */
void ZerarNatrix(Matrix *);


/*
 * MatrixPrint
 *
 * Mostra a matrix no terminal
 *
 * Parametros:
 * Matrix*
 */
void MatrixPrint(Matrix *);


/*
 * MatrixFree
 *
 * Desaloca a matriz
 *
 * Parametros:
 * Matrix**
 */
void MatrixFree(Matrix **);


/*
 * MatrixFree
 *
 * Muda todos os valore da matrix para `valor`
 *
 * Parametros:
 * Matrix*
 * Valor: int
 */
void SetMatrix(Matrix *, int);


/*
 * MatrixCompare
 *
 * Detecta se duas matrizes são iguais
 *
 * Parametros:
 * Matrix*
 * Matrix
 *
 * Retorno:
 * Inteiro
 * 0 se forem diferentes
 * 1 se forem iguais
 */
int MatrixCompare(Matrix *, Matrix *);

/*
 * Funcao para mostra a matriz de um jeito bonito
 */
void pretty_print_matrix_value(int);

#endif // __MATRIX_H__
