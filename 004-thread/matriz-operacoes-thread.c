//
// Created by Gustavo Diel on 09/10/19.
//

#include "matriz-operacoes-thread.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz_struct.h"


void *mult_normal_job(void *raw_data) {
    mult_normal_package *data = (mult_normal_package *) raw_data;

    for (int i = data->linha; i < data->lin_a; i += data->threads) {
        for (int j = 0; j < data->col_b; j++) {
            for (int k = 0; k < data->col_a; ++k) {
                data->result[i][j] += data->first[i][k] * data->second[k][j];
            }
        }
    }

    return NULL;
}

mymatriz *multiplicarTh(mymatriz *matrixA, mymatriz *matrixB, int thread_count) {
    mult_normal_package mult_content[thread_count];
    pthread_t threads[thread_count];

    mymatriz *resultado = (mymatriz *) malloc(sizeof(mymatriz));

    resultado->lin = matrixA->lin;
    resultado->col = matrixB->col;

    malocar(resultado);
    mzerar(resultado);

    int i;

    for (i = 0; i < thread_count; ++i) {
        mult_content[i].first = matrixA->matriz;
        mult_content[i].second = matrixB->matriz;
        mult_content[i].linha = i;
        mult_content[i].threads = thread_count;
        mult_content[i].result = resultado->matriz;
        mult_content[i].col_a = matrixA->col;
        mult_content[i].col_b = matrixB->col;
        mult_content[i].lin_a = matrixA->lin;

        pthread_create(&threads[i], NULL, mult_normal_job, (void *) (mult_content + i));
    }

    for (i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    return resultado;
}

void *mult_block_thread_job(void *raw_data) {
    mult_block_normal_package *matriz = (mult_block_normal_package *) raw_data;

    mmsubmatriz(matriz->first, matriz->second, matriz->result);
}

mymatriz *multiplicarThblocos(mymatriz *matrixA, mymatriz *matrixB, int thread_count) {
    matriz_bloco_t **Vsubmat_a = NULL, **Vsubmat_b = NULL, **Vsubmat_c = NULL;

    mult_block_normal_package packages[thread_count];
    pthread_t threads[thread_count];

    Vsubmat_a = particionar_matriz(matrixA->matriz, matrixA->lin, matrixA->col, 1, thread_count);
    Vsubmat_b = particionar_matriz(matrixB->matriz, matrixB->lin, matrixB->col, 0, thread_count);

    Vsubmat_c = csubmatrizv2(matrixA->lin, matrixB->col, thread_count);

    mymatriz *resultado = (mymatriz *) malloc(sizeof(mymatriz));

    resultado->lin = matrixA->lin;
    resultado->col = matrixB->col;

    malocar(resultado);
    mzerar(resultado);

    for (int i = 0; i < thread_count; i++) {
        packages[i].first = Vsubmat_a[i];
        packages[i].second = Vsubmat_b[i];
        packages[i].result = Vsubmat_c[i];

        pthread_create(&threads[i], NULL, mult_block_thread_job, (void *) (packages + i));
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);

        mymatriz *temp = msomar(resultado, Vsubmat_c[i]->matriz, 0);

        resultado = temp;
    }

    return resultado;
}

