#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matriz_struct.h"
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"

#include "matriz-operacoes-thread.h"

#define MAX_EXECUTIONS 3
#define NUM_THREADS 2

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // DECLARAÇÃO de VARIÁVEIS
    mymatriz mat_a, mat_b;
    mymatriz **mmultbloco, **mmult, **thread_mmultbloco, **thread_mmult;
    char filename[100];
    FILE *fmat;
    int nr_line;
    int *vet_line = NULL;
    int N, M, La, Lb;
    double start_time, end_time;

    double seq_normal_times[MAX_EXECUTIONS];
    double thr_normal_times[MAX_EXECUTIONS];
    double seq_bloco_times[MAX_EXECUTIONS];
    double thr_bloco_times[MAX_EXECUTIONS];

    int num;

    matriz_bloco_t **Vsubmat_a = NULL, **Vsubmat_b = NULL, **Vsubmat_c = NULL;

    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    if (argc != 3) {
        printf("ERRO: Numero de parametros %s <matriz_a> <matriz_b>\n", argv[0]);
        exit(1);
    }

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //                Leitura da Matriz A (arquivo)
    fmat = fopen(argv[1], "r");
    if (fmat == NULL) {
        printf("Error: Na abertura dos arquivos.");
        exit(1);
    }
    extrai_parametros_matriz(fmat, &N, &La, &vet_line, &nr_line);
    //return 1;
    mat_a.matriz = NULL;
    mat_a.lin = N;
    mat_a.col = La;
    if (malocar(&mat_a)) {
        printf("ERROR: Out of memory\n");
    }
    filein_matriz(mat_a.matriz, N, La, fmat, vet_line, nr_line);
    free(vet_line);
    fclose(fmat);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //               Leitura da Matriz B (arquivo)
    fmat = fopen(argv[2], "r");
    if (fmat == NULL) {
        printf("Error: Na abertura dos arquivos.");
        exit(1);
    }
    extrai_parametros_matriz(fmat, &Lb, &M, &vet_line, &nr_line);
    mat_b.matriz = NULL;
    mat_b.lin = Lb;
    mat_b.col = M;
    if (malocar(&mat_b)) {
        printf("ERROR: Out of memory\n");
    }
    filein_matriz(mat_b.matriz, Lb, M, fmat, vet_line, nr_line);
    free(vet_line);
    fclose(fmat);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    printf("\n%%%%%%%%%%%%%%%%\n");

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //               Operações de Multiplicação

    num = MAX_EXECUTIONS;
    mmult = (mymatriz **) malloc(sizeof(mymatriz *) * MAX_EXECUTIONS);
    while (num--) {
        int real_num = MAX_EXECUTIONS - 1 - num;
        printf("\n ##### multiplicar_t%d de Matrizes #####\n", real_num);
        start_time = wtime();
        mmult[real_num] = mmultiplicar(&mat_a, &mat_b, 4);
        end_time = wtime();

        if (!mmult[real_num]) {
            exit(1);
        }

        printf("\tRuntime: %f\n", end_time - start_time);
        sprintf(filename, "mult_t%d.result", real_num);
        fmat = fopen(filename, "w");
        fileout_matriz(mmult[real_num], fmat);
        fclose(fmat);

        seq_normal_times[real_num] = end_time - start_time;
    }

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //               Operações de Multiplicação ( THREAD )

    num = MAX_EXECUTIONS;
    thread_mmult = (mymatriz **) malloc(sizeof(mymatriz *) * MAX_EXECUTIONS);

    while (num--) {
        int real_num = MAX_EXECUTIONS - 1 - num;
        printf("\n ##### THREAD - thread_multiplicar_t%d de Matrizes #####\n", real_num);
        start_time = wtime();
        thread_mmult[0] = multiplicarTh(&mat_a, &mat_b, NUM_THREADS);
        end_time = wtime();

        if (!thread_mmult[0]) {
            exit(1);
        }

        printf("\tRuntime: %f\n", end_time - start_time);
        sprintf(filename, "mult_thread_t%d.result", real_num);
        fmat = fopen(filename, "w");
        fileout_matriz(thread_mmult[0], fmat);
        fclose(fmat);

        thr_normal_times[real_num] = (end_time - start_time);
    }
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //               Operações de Multiplicação (em bloco)
    num = MAX_EXECUTIONS;
    mmultbloco = (mymatriz **) malloc(sizeof(mymatriz *) * MAX_EXECUTIONS);

    while (num--) {
        int real_num = MAX_EXECUTIONS - 1 - num;
        printf("\n ##### multiplicar_block_t%d de Matrizes #####\n", real_num);
        start_time = wtime();

        Vsubmat_a = particionar_matriz(mat_a.matriz, N, La, 1, 2);
        Vsubmat_b = particionar_matriz(mat_b.matriz, Lb, M, 0, 2);
        Vsubmat_c = csubmatrizv2(N, M, 2);

        mmsubmatriz(Vsubmat_a[0], Vsubmat_b[0], Vsubmat_c[0]);
        mmsubmatriz(Vsubmat_a[1], Vsubmat_b[1], Vsubmat_c[1]);

        mmultbloco[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);

        end_time = wtime();

        if (!mmultbloco[0]) {
            exit(1);
        }
        printf("\tRuntime: %f\n", end_time - start_time);
        sprintf(filename, "mult_block_t%d.result", real_num);
        fmat = fopen(filename, "w");
        fileout_matriz(mmultbloco[0], fmat);
        fclose(fmat);

        seq_bloco_times[real_num] = end_time - start_time;
    }
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //               Operações de Multiplicação (em bloco)
    num = MAX_EXECUTIONS;
    thread_mmultbloco = (mymatriz **) malloc(sizeof(mymatriz *) * MAX_EXECUTIONS);

    while (num--) {
        int real_num = MAX_EXECUTIONS - 1 - num;
        printf("\n ##### THREAD - thread_multiplicar_block_t%d de Matrizes #####\n", real_num);
        start_time = wtime();

        thread_mmultbloco[real_num] = multiplicarThblocos(&mat_a, &mat_b, NUM_THREADS);

        end_time = wtime();

        if (!thread_mmultbloco[real_num]) {
            exit(1);
        }
        printf("\tRuntime: %f\n", end_time - start_time);
        sprintf(filename, "mult_block_thread_t%d.result", real_num);
        fmat = fopen(filename, "w");
        fileout_matriz(thread_mmultbloco[real_num], fmat);
        fclose(fmat);

        thr_bloco_times[real_num] = end_time - start_time;
    }
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //              Comparação dos resultados
    printf("\n ##### Comparação dos resultados da Multiplicação de matrizes #####\n");

    for (int i = 0; i < MAX_EXECUTIONS; i++) {
        printf("[mult_t%d vs mult_thread_t%d]\t", i);
        mcomparar(mmult[0], thread_mmult[0]);
    }

    for (int i = 0; i < MAX_EXECUTIONS; i++) {
        printf("[mult_t%d vs mult_block_t%d]\t", i);
        mcomparar(mmult[0], mmultbloco[0]);
    }

    for (int i = 0; i < MAX_EXECUTIONS; i++) {
        printf("[mult_t%d vs mult_block_thread_t%d]\t", i);
        mcomparar(mmult[0], thread_mmultbloco[0]);
    }
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //                   Liberação de memória
    mliberar(mmult[0]);
    free(mmult[0]);
    mliberar(mmultbloco[0]);
    free(mmultbloco[0]);

    mliberar(&mat_a);
    mliberar(&mat_b);
    free(mmult);
    free(mmultbloco);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    return 0;
}
