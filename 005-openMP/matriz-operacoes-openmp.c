//
// Created by Gustavo Diel on 09/10/19.
//

#include "matriz-operacoes-openmp.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz_struct.h"

mymatriz *multiplicarOpenMP(mymatriz *matrixA, mymatriz *matrixB, int thread_count) {
    mymatriz *resultado = (mymatriz *) malloc(sizeof(mymatriz));

    resultado->lin = matrixA->lin;
    resultado->col = matrixB->col;

    malocar(resultado);
    mzerar(resultado);

    int thread;

    omp_set_num_threads(thread_count);

#pragma omp parallel for shared(resultado) private(thread) schedule(dynamic)
    for (thread = 0; thread < thread_count; ++thread) {
        for (int i = thread; i < matrixA->lin; i += thread_count) {
            for (int j = 0; j < matrixB->col; j++) {
                for (int k = 0; k < matrixA->col; ++k) {
                    resultado->matriz[i][j] += matrixA->matriz[i][k] * matrixB->matriz[k][j];
                }
            }
        }
    }

    return resultado;
}

mymatriz *multiplicarOpenMPBlocos(mymatriz *matrixA, mymatriz *matrixB, int thread_count) {
    matriz_bloco_t **Vsubmat_a = NULL, **Vsubmat_b = NULL, **Vsubmat_c = NULL;

    Vsubmat_a = particionar_matriz(matrixA->matriz, matrixA->lin, matrixA->col, 1, thread_count);
    Vsubmat_b = particionar_matriz(matrixB->matriz, matrixB->lin, matrixB->col, 1, thread_count);

    Vsubmat_c = csubmatrizv2(matrixA->lin, matrixB->col, thread_count);

    mymatriz *resultado = (mymatriz *) malloc(sizeof(mymatriz));

    resultado->lin = matrixA->lin;
    resultado->col = matrixB->col;

    malocar(resultado);
    mzerar(resultado);

    int thread;

    omp_set_num_threads(thread_count);

#pragma omp parallel for shared(Vsubmat_a, Vsubmat_b, Vsubmat_c) private(thread) schedule(dynamic)
    for (thread = 0; thread < thread_count; ++thread) {
        mmsubmatriz(Vsubmat_a[thread], Vsubmat_b[thread], Vsubmat_c[thread]);
    }

    for (int i = 0; i < thread_count; i++) {
        mymatriz *temp = msomar(resultado, Vsubmat_c[i]->matriz, 0);

        resultado = temp;
    }

    return resultado;
}
