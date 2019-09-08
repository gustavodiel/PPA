#include "matriz-operacoesv3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "matrizv3.h"

int matrizesNulas(mymatriz* a, mymatriz *b){
  return (a == NULL || b == NULL);
}

int podeSomarMatrizes(mymatriz *a, mymatriz *b) {
  return a->col == b->col && a->lin == b->lin;
}

void somarIJ(mymatriz* result, mymatriz* a, mymatriz* b, int i, int j) {
  result->matriz[i][j] = a->matriz[i][j] + b->matriz[i][j];
}

void somarJI(mymatriz* result, mymatriz* a, mymatriz* b, int i, int j) {
  result->matriz[j][i] = a->matriz[j][i] + b->matriz[j][i];
}

mymatriz *msomar (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
  if (matrizesNulas(mat_a, mat_b)) {
    printf("Uma das matrizes veio nula!\n");
    return NULL;
  }

  if (!podeSomarMatrizes(mat_a, mat_b)) {
    printf("Matrizes de tamanhos diferentes!!\n");
    printf("(%d,%d) x (%d,%d)\n", mat_a->lin, mat_a->col, mat_b->lin, mat_b->col);
    return NULL;
  }

  int outterLoop;
  int innerLoop;

  int i, j;

  void (*somarMatriz)(mymatriz*, mymatriz*, mymatriz*, int, int);

  switch (tipo)
  {
  case 0:
    somarMatriz = &somarIJ;
    outterLoop = mat_a->lin;
    innerLoop = mat_a->col;
    break;
  case 1:
    somarMatriz = &somarJI;
    outterLoop = mat_a->col;
    innerLoop = mat_a->lin;
    break;
  default:
    printf("Tipo desconhecido!\n\t0 para IJ\n\t1 para JI\n");
    return NULL;
  }

  mymatriz *result = (mymatriz*)malloc(sizeof(mymatriz));
  result->col = mat_a->col, result->lin = mat_a->lin;

  malocar(result);

  for (i = 0; i < outterLoop; ++i) {
    for (j = 0; j < innerLoop; ++j) {
      (*somarMatriz)(result, mat_a, mat_b, i, j);
    }
  }

  return result;
}

int podeMultiplicarMatrizes(mymatriz *a, mymatriz *b) {
  return a->col == b->lin;
}

void multIJK(mymatriz* result, mymatriz* a, mymatriz* b, int i, int j, int k) {
  result->matriz[i][j] += a->matriz[i][k] * b->matriz[j][k];
}

void multIKJ(mymatriz* result, mymatriz* a, mymatriz* b, int i, int j, int k) {
  result->matriz[i][k] += a->matriz[i][j] * b->matriz[k][j];
}

void multJKI(mymatriz* result, mymatriz* a, mymatriz* b, int i, int j, int k) {
  result->matriz[j][k] += a->matriz[j][i] * b->matriz[k][i];
}

void multJIK(mymatriz* result, mymatriz* a, mymatriz* b, int i, int j, int k) {
  result->matriz[j][i] += a->matriz[j][k] * b->matriz[i][k];
}

void multKJI(mymatriz* result, mymatriz* a, mymatriz* b, int i, int j, int k) {
  result->matriz[k][j] += a->matriz[k][i] * b->matriz[j][i];
}

void multKIJ(mymatriz* result, mymatriz* a, mymatriz* b, int i, int j, int k) {
  result->matriz[k][i] += a->matriz[k][j] * b->matriz[i][j];
}

mymatriz *mmultiplicar (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
  if (matrizesNulas(mat_a, mat_b)) {
    printf("Uma das matrizes veio nula!\n");
    return NULL;
  }

  if (!podeMultiplicarMatrizes(mat_a, mat_b)) {
    printf("Matrizes de tamanhos incompatíveis!!\n");
    printf("(%d,%d) x (%d,%d)\n", mat_a->lin, mat_a->col, mat_b->lin, mat_b->col);
    return NULL;
  }

  int outterLoop;
  int middleLoop;
  int innerLoop;

  int final_lin = mat_a->lin;
  int final_col = mat_b->col;

  int i, j, k;

  void (*multiplicarMatriz)(mymatriz*, mymatriz*, mymatriz*, int, int, int);

  switch (tipo)
  {
  case 0:
    multiplicarMatriz = &multIJK;
    outterLoop = mat_a->lin;
    middleLoop = mat_b->col;
    innerLoop = mat_b->col;
    break;

  case 1:
    multiplicarMatriz = &multIKJ;
    outterLoop = mat_a->lin;
    middleLoop = mat_b->col;
    innerLoop = mat_b->col;
    break;

  case 2:
    multiplicarMatriz = &multJKI;
    outterLoop = mat_b->col;
    middleLoop = mat_b->col;
    innerLoop = mat_a->lin;
    break;

  case 3:
    multiplicarMatriz = &multJIK;
    outterLoop = mat_b->col;
    middleLoop = mat_a->lin;
    innerLoop = mat_b->col;
    break;

  case 4:
    multiplicarMatriz = &multKJI;
    outterLoop = mat_b->col;
    middleLoop = mat_b->col;
    innerLoop = mat_a->lin;
    break;

  case 5:
    multiplicarMatriz = &multKIJ;
    outterLoop = mat_b->col;
    middleLoop = mat_a->lin;
    innerLoop = mat_b->col;
    break;

  default:
    printf(
      "Tipo desconhecido!"
      "\n\t0 para IJK"
      "\n\t1 para IKJ"
      "\n\t2 para JKI"
      "\n\t3 para JIK"
      "\n\t4 para KJI"
      "\n\t5 para KIJ"
      "\n"
    );
    return NULL;
  }

  mymatriz *result = (mymatriz*)malloc(sizeof(mymatriz));
  result->col = final_col, result->lin = final_lin;

  malocar(result);
  mzerar(result);

  for (i = 0; i < outterLoop; ++i) {
    for (j = 0; j < middleLoop; ++j) {
      for (k = 0; k < innerLoop; ++k) {
        (*multiplicarMatriz)(result, mat_a, mat_b, i, j, k);
      }
    }
  }

  return result;
}
