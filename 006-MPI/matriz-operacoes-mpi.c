//
// Created by Gustavo Diel on 09/10/19.
//

#include "matriz-operacoes-mpi.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <mpi.h>

#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz_struct.h"

mymatriz *multiplicarMPI(mymatriz *matrixA, mymatriz *matrixB, int rank, int size) {
    mymatriz *resultado = (mymatriz *) malloc(sizeof(mymatriz));

    resultado->lin = matrixA->lin;
    resultado->col = matrixB->col;

    malocar(resultado);
    mzerar(resultado);

    for (int i = rank; i < matrixA->lin; i += size) {
        for (int j = 0; j < matrixB->col; j++) {
            for (int k = 0; k < matrixA->col; ++k) {
                resultado->matriz[i][j] += matrixA->matriz[i][k] * matrixB->matriz[k][j];
            }
        }
    }

    return resultado;
}

void mergeMatrixBloco(mymatriz *mat, int rank, int size) {
    if (rank == 0) {
        int *data = (int*)malloc(sizeof(int) * mat->lin * mat->col);
        MPI_Status status;

        for (int i = 1; i < size; ++i) {
            MPI_Recv(data, mat->lin * mat->col, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < mat->lin * mat->col; i++) {
                mat->matriz[0][i] += data[i];
            }
        }
    } else {
        MPI_Send(mat->matriz[0], mat->lin * mat->col, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}

mymatriz *multiplicarMPIBlocos(mymatriz *matrixA, mymatriz *matrixB, int rank, int size) {
    matriz_bloco_t **Vsubmat_a = NULL, **Vsubmat_b = NULL, **Vsubmat_c = NULL;

    Vsubmat_a = particionar_matriz(matrixA->matriz, matrixA->lin, matrixA->col, 0, size);
    Vsubmat_b = particionar_matriz(matrixB->matriz, matrixB->lin, matrixB->col, 0, size);

    Vsubmat_c = csubmatrizv2(matrixA->lin, matrixB->col, size);

    mmsubmatriz(Vsubmat_a[rank], Vsubmat_b[rank], Vsubmat_c[rank]);

    // receber aqui
    if (rank == 0) {

        mymatriz *resultado = (mymatriz *) malloc(sizeof(mymatriz));

        resultado->lin = matrixA->lin;
        resultado->col = matrixB->col;

        malocar(resultado);
        mzerar(resultado);

        for (int indx = 0; indx < resultado->lin * resultado->col; indx++) {
            resultado->matriz[0][indx] += Vsubmat_c[rank]->matriz->matriz[0][indx];
        }

        mergeMatrixBloco(resultado, rank, size);

        return resultado;
    } else {
        mergeMatrixBloco(Vsubmat_c[rank]->matriz, rank, size);
    }

    return NULL;
}
