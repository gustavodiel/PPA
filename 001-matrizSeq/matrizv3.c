#include "matrizv3.h"

int mzerar(mymatriz *matriz) {
    if (matriz == NULL) {
        perror("[mzerar] Matriz enviada é nula!\n");
        return 1;
    }

    for (int i = 0; i < matriz->lin; ++i) {
        for (int j = 0; j < matriz->col; ++j) {
            matriz->matriz[i][j] = 0;
        }
    }

    return 0;
}

int mliberar(mymatriz *matriz) {
    if (matriz != NULL) {
        free(matriz->matriz[0]);
        free(matriz->matriz);
    }

    return 0;
}

int malocar(mymatriz *matriz) {
    if (matriz == NULL) {
        perror("[malocar] A matriz é nula!\n");
        return 1;
    }

    /*
        O código a seguir é referente à alocação contínua da matriz na memória
        O jeito que fica na memória é um vetor unidimencional enorme (o segundo malloc),
        e um segundo vetor (primeiro malloc) utlizado para mapear o inicio de cada linha (feito no for)
     */
    matriz->matriz = (int**)malloc(sizeof(int*) * matriz->lin);

    int *temp = (int*)malloc(sizeof(int) * matriz->lin * matriz->col);

    for (int i = 0; i < matriz->lin; ++i) {
      matriz->matriz[i] = (i * matriz->col + temp);
    }

    return 0;
}

int mimprimir(mymatriz *matriz) {
    if (matriz == NULL) {
        perror("[mimprimir] A matriz é nula!\n");
        return 1;
    }

    printf("\n");
    for (int i = 0; i < matriz->lin; ++i) {
        for (int j = 0; j < matriz->col; ++j) {
            printf("%3i ", matriz->matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}

int mgerar(mymatriz *matriz, int valor) {
    if (matriz == NULL) {
        perror("[mgerar] A matriz é nula!\n");
        return 1;
    }

    for (int i = 0; i < matriz->lin; ++i) {
        for (int j = 0; j < matriz->col; ++j) {
            matriz->matriz[i][j] = valor;
        }
    }

    return 0;
}

int mcomparar(mymatriz *matriz_a, mymatriz *matriz_b) {
    if (matriz_a == NULL) {
        perror("[mcomparar] A matriz A é nula!\n");
        return 1;
    }

    if (matriz_b == NULL) {
        perror("[mcomparar] A matriz B é nula!\n");
        return 1;
    }

    int col = matriz_a->col;
    int lin = matriz_a->lin;

    if (col != matriz_b->col || lin != matriz_b->lin) {
        printf(
            "O tamanho das duas matrizes são diferentes!\n"
            "\tA: %d x %d\n"
            "\tB: %d x %d\n",
            matriz_a->lin, matriz_a->col,
            matriz_b->lin, matriz_b->col
        );

        return 1;
    }

    for (int i = 0; i < lin; ++i) {
        for (int j = 0; j < col; ++j) {
            if (matriz_a->matriz[i][j] != matriz_b->matriz[i][j]) {
                printf(
                    "As matrizes são diferentes!\n"
                    "\tA[%d][%d] = %d\n"
                    "\tB[%d][%d] = %d\n",
                    i, j, matriz_a->matriz[i][j],
                    i, j, matriz_b->matriz[i][j]
                );

                return 1;
            }
        }
    }

    printf( "Matrizes iguais!\n");

    return 0;
}
