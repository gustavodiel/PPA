//
// Created by Gustavo Diel on 09/10/19.
//

#ifndef INC_004_THREAD_MATRIZ_OPERACOES_THREAD_H
#define INC_004_THREAD_MATRIZ_OPERACOES_THREAD_H

#include "matrizv3.h"

typedef struct {
    mymatriz* a, *b, *result;
    int linha, threads;
} mult_normal_package;

typedef struct {
    matriz_bloco_t *first;
    matriz_bloco_t *second;
    matriz_bloco_t *result;
} mult_block_normal_package;

mymatriz* multiplicarTh(mymatriz*, mymatriz*, int);
void* mult_normal_job(void*);


mymatriz* multiplicarThblocos(mymatriz*, mymatriz*, int);
void* mult_block_thread_job(void*);

#endif //INC_004_THREAD_MATRIZ_OPERACOES_THREAD_H
