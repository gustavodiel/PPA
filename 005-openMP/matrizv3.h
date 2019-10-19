#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matriz_struct.h"

// gerência de matrizes
int malocar(mymatriz *matriz);
int mgerar(mymatriz *matriz, int valor);
int mimprimir(mymatriz *matriz);
int mzerar(mymatriz *matriz);
int mliberar(mymatriz *matriz);
int mcomparar(mymatriz *mat_a, mymatriz *mat_b);
