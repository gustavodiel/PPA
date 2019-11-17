//
// Created by Gustavo Diel on 09/10/19.
//

#ifndef INC_006_MPI_MATRIZ_OPERACOES_THREAD_H
#define INC_006_MPI_MATRIZ_OPERACOES_THREAD_H

#include "matrizv3.h"

mymatriz* multiplicarMPI(mymatriz*, mymatriz*, int rank, int size);

mymatriz* multiplicarMPIBlocos(mymatriz*, mymatriz*, int rank, int size);

#endif //INC_006_MPI_MATRIZ_OPERACOES_THREAD_H
