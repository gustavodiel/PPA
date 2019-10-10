#include "matriz-operacoesv3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "matrizv3.h"

int matrizesNulas(void *a, void *b)
{
  return (!a || !b);
}

int podeSomarMatrizes(mymatriz *a, mymatriz *b)
{
  return a->col == b->col && a->lin == b->lin;
}

int mmsubmatriz(matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc)
{

  if (!mat_suba || !mat_subb || !mat_subc)
  {
    printf("[mmsubmatriz] ERROR: Out of memory\n");
    return 1;
  }

  int i, j, k;
  for (i = mat_suba->bloco->lin_inicio; i < mat_suba->bloco->lin_fim; i++)
  {
    for (k = mat_suba->bloco->col_inicio; k < mat_suba->bloco->col_fim; k++)
    {
      for (j = mat_subb->bloco->col_inicio; j < mat_subb->bloco->col_fim; j++)
      {
        mat_subc->matriz->matriz[i][j] += mat_suba->matriz->matriz[i][k] * mat_subb->matriz->matriz[k][j];
      }
    }
  }
  return 0;
}

matriz_bloco_t **csubmatrizv2(int mat_lin, int mat_col, int divisor)
{
  matriz_bloco_t **block = NULL;
  block = (matriz_bloco_t **)calloc(divisor, sizeof(matriz_bloco_t *));

  if (!block)
  {
    printf("[csubmatrizv2] ERROR: Out of memory\n");
    return NULL;
  }

  for (int i = 0; i < divisor; i++)
  {
    block[i] = (matriz_bloco_t *)malloc(sizeof(matriz_bloco_t));
    block[i]->bloco = (bloco_t *)malloc(sizeof(bloco_t));
  }

  for (int i = 0; i < divisor; i++)
  {
    mymatriz *matBl = (mymatriz *)malloc(sizeof(mymatriz));
    matBl->lin = mat_lin;
    matBl->col = mat_col;

    malocar(matBl);
    mzerar(matBl);

    block[i]->matriz = matBl;
    block[i]->bloco->lin_inicio = 0;
    block[i]->bloco->lin_fim = mat_lin;
    block[i]->bloco->col_inicio = 0;
    block[i]->bloco->col_fim = mat_col;
  }

  return block;
}

matriz_bloco_t **particionar_matriz(int **matriz, int mat_lin, int mat_col, int orientacao, int divisor)
{
  matriz_bloco_t **block = NULL;
  block = (matriz_bloco_t **)calloc(divisor, sizeof(matriz_bloco_t *));

  if (matrizesNulas(matriz, block))
  {
    printf("[particionar_matriz] ERROR: Out of memory\n");
    return NULL;
  }

  for (int i = 0; i < divisor; i++)
  {
    block[i] = (matriz_bloco_t *)malloc(sizeof(matriz_bloco_t));
    block[i]->bloco = (bloco_t *)malloc(sizeof(bloco_t));
  }

  mymatriz *megaMatriz = (mymatriz *)malloc(sizeof(mymatriz));
  megaMatriz->matriz = matriz;

  if (orientacao == 0)
  {
    int desloc = mat_lin / divisor;
    for (int i = 0; i < divisor; i++)
    {
      block[i]->matriz = megaMatriz;
      block[i]->bloco->lin_inicio = i * desloc;
      block[i]->bloco->lin_fim = (i + 1) * desloc;
      block[i]->bloco->col_inicio = 0;
      block[i]->bloco->col_fim = mat_col;
    }
    block[divisor - 1]->bloco->lin_fim = mat_lin;
  }
  else
  {
    int desloc = mat_col / divisor;
    for (int i = 0; i < divisor; i++)
    {
      block[i]->matriz = megaMatriz;
      block[i]->bloco->lin_inicio = 0;
      block[i]->bloco->lin_fim = mat_lin;
      block[i]->bloco->col_inicio = i * desloc;
      block[i]->bloco->col_fim = (i + 1) * desloc;
    }
    block[divisor - 1]->bloco->col_fim = mat_col;
  }
  return block;
}

void somarIJ(mymatriz *result, mymatriz *a, mymatriz *b, int i, int j)
{
  result->matriz[i][j] = a->matriz[i][j] + b->matriz[i][j];
}

void somarJI(mymatriz *result, mymatriz *a, mymatriz *b, int i, int j)
{
  result->matriz[j][i] = a->matriz[j][i] + b->matriz[j][i];
}

mymatriz *msomar(mymatriz *mat_a, mymatriz *mat_b, int tipo)
{
  if (matrizesNulas(mat_a, mat_b))
  {
    printf("Uma das matrizes veio nula!\n");
    return NULL;
  }

  if (!podeSomarMatrizes(mat_a, mat_b))
  {
    printf("Matrizes de tamanhos diferentes!!\n");
    printf("(%d,%d) x (%d,%d)\n", mat_a->lin, mat_a->col, mat_b->lin, mat_b->col);
    return NULL;
  }

  int outterLoop;
  int innerLoop;

  int i, j;

  void (*somarMatriz)(mymatriz *, mymatriz *, mymatriz *, int, int);

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

  mymatriz *result = (mymatriz *)malloc(sizeof(mymatriz));
  result->col = mat_a->col, result->lin = mat_a->lin;

  malocar(result);

  for (i = 0; i < outterLoop; ++i)
  {
    for (j = 0; j < innerLoop; ++j)
    {
      (*somarMatriz)(result, mat_a, mat_b, i, j);
    }
  }

  return result;
}

int podeMultiplicarMatrizes(mymatriz *a, mymatriz *b)
{
  return a->col == b->lin;
}

void multIJK(mymatriz *result, mymatriz *a, mymatriz *b, int i, int j, int k)
{
  result->matriz[i][j] += a->matriz[i][k] * b->matriz[k][j];
}

void multIKJ(mymatriz *result, mymatriz *a, mymatriz *b, int i, int j, int k)
{
  result->matriz[i][k] += a->matriz[i][j] * b->matriz[j][k];
}

void multJKI(mymatriz *result, mymatriz *a, mymatriz *b, int i, int j, int k)
{
  result->matriz[j][k] += a->matriz[j][i] * b->matriz[i][k];
}

void multJIK(mymatriz *result, mymatriz *a, mymatriz *b, int i, int j, int k)
{
  result->matriz[j][i] += a->matriz[j][k] * b->matriz[k][i];
}

void multKJI(mymatriz *result, mymatriz *a, mymatriz *b, int i, int j, int k)
{
  result->matriz[k][j] += a->matriz[k][i] * b->matriz[i][j];
}

void multKIJ(mymatriz *result, mymatriz *a, mymatriz *b, int i, int j, int k)
{
  result->matriz[k][i] += a->matriz[k][j] * b->matriz[j][i];
}

mymatriz *mmultiplicar(mymatriz *mat_a, mymatriz *mat_b, int tipo)
{
  if (matrizesNulas(mat_a, mat_b))
  {
    printf("Uma das matrizes veio nula!\n");
    return NULL;
  }

  if (!podeMultiplicarMatrizes(mat_a, mat_b))
  {
    printf("Matrizes de tamanhos incompatíveis!!\n");
    printf("(%d,%d) x (%d,%d)\n", mat_a->lin, mat_a->col, mat_b->lin, mat_b->col);
    return NULL;
  }

  int outterLoop = mat_a->lin;
  int middleLoop = mat_b->lin;
  int innerLoop = mat_b->col;

  int final_lin = mat_a->lin;
  int final_col = mat_b->col;

  int i, j, k;

  mymatriz *result = (mymatriz *)malloc(sizeof(mymatriz));
  result->col = final_col, result->lin = final_lin;

  malocar(result);
  mzerar(result);

  for (i = 0; i < outterLoop; ++i)
  {
    for (k = 0; k < innerLoop; ++k)
    {
      for (j = 0; j < middleLoop; ++j)
      {
        result->matriz[i][k] += mat_a->matriz[i][j] * mat_b->matriz[j][k];
      }
    }
  }

  return result;
}
