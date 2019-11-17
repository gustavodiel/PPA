#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "matriz_struct.h"
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"

#include "matriz-operacoes-mpi.h"

#define MAX_EXECUTIONS 10

void workerRun(int rank, int size);
void masterRun(int argc, char *argv[], int rank, int size);
void broadcastMatrix(mymatriz *mat, int rank, int size);
void mergeMatrix(mymatriz*, int, int);



// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {
    MPI_Init(NULL, NULL);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        masterRun(argc, argv, rank, size);
    } else {
        workerRun(rank, size);
    }

    MPI_Finalize();
}

void broadcastMatrix(mymatriz *mat, int rank, int size) {
    int lin, col;
    int *data;

    if (rank == 0) {
        lin = mat->lin;
        col = mat->col;
        data = mat->matriz[0];
    }

    MPI_Bcast(&lin, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&col, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        data = (int*)malloc(sizeof(int) * lin * col);
    }

    MPI_Bcast(data, lin * col, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        mat->lin = lin;
        mat->col = col;

        mat->matriz = (int**)malloc(sizeof(int*) * mat->lin);

        for (int i = 0; i < mat->lin; ++i) {
        mat->matriz[i] = (i * mat->col + data);
        }
    }
}


void mergeMatrix(mymatriz *mat, int rank, int size) {
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

void workerRun(int rank, int size) {
    mymatriz mat_a;
    mymatriz mat_b;

    // Receive matrixA
    broadcastMatrix(&mat_a, rank, size);

    // Receive matrixB
    broadcastMatrix(&mat_b, rank, size);

    // Multiplicar Normal
    int num = MAX_EXECUTIONS;

    while (num--) {
        MPI_Barrier(MPI_COMM_WORLD);

        mymatriz* result = multiplicarMPI(&mat_a, &mat_b, rank, size);

        mergeMatrix(result, rank, size);
    }

    // Multiplicar Bloco
    num = MAX_EXECUTIONS;

    while (num--) {
        MPI_Barrier(MPI_COMM_WORLD);

        multiplicarMPIBlocos(&mat_a, &mat_b, rank, size);
    }
}


void masterRun(int argc, char *argv[], int rank, int size) {

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


    printf("\nSending Matrixes\n");
    broadcastMatrix(&mat_a, rank, size);
    broadcastMatrix(&mat_b, rank, size);


    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //               Operações de Multiplicação

    num = MAX_EXECUTIONS;
    mmult = (mymatriz **) malloc(sizeof(mymatriz *) * MAX_EXECUTIONS);
    while (num--) {
        int real_num = MAX_EXECUTIONS - 1 - num;
        printf("\n ##### multiplicar_t%d de Matrizes #####\n", real_num);
        start_time = wtime();
        mmult[real_num] = mmultiplicar(&mat_a, &mat_b, 1);
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

        mmultbloco[real_num] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 0);

        end_time = wtime();

        if (!mmultbloco[real_num]) {
            exit(1);
        }
        printf("\tRuntime: %f\n", end_time - start_time);
        sprintf(filename, "mult_block_t%d.result", real_num);
        fmat = fopen(filename, "w");
        fileout_matriz(mmultbloco[real_num], fmat);
        fclose(fmat);

        seq_bloco_times[real_num] = end_time - start_time;
    }
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%


    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //               Operações de Multiplicação ( THREAD )

    num = MAX_EXECUTIONS;
    thread_mmult = (mymatriz **) malloc(sizeof(mymatriz *) * MAX_EXECUTIONS);

    while (num--) {
        MPI_Barrier(MPI_COMM_WORLD);

        int real_num = MAX_EXECUTIONS - 1 - num;
        printf("\n ##### MPI THREAD - thread_multiplicar_t%d de Matrizes #####\n", real_num);
        start_time = wtime();
        thread_mmult[real_num] = multiplicarMPI(&mat_a, &mat_b, rank, size);
        mergeMatrix(thread_mmult[real_num], rank, size);

        end_time = wtime();

        if (!thread_mmult[real_num]) {
            exit(1);
        }

        printf("\tRuntime: %f\n", end_time - start_time);
        sprintf(filename, "mult_thread_t%d.result", real_num);
        fmat = fopen(filename, "w");
        fileout_matriz(thread_mmult[real_num], fmat);
        fclose(fmat);

        thr_normal_times[real_num] = (end_time - start_time);
    }
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //               Operações de Multiplicação (em bloco)
    num = MAX_EXECUTIONS;
    thread_mmultbloco = (mymatriz **) malloc(sizeof(mymatriz *) * MAX_EXECUTIONS);

    while (num--) {
        MPI_Barrier(MPI_COMM_WORLD);

        int real_num = MAX_EXECUTIONS - 1 - num;
        printf("\n ##### MPI THREAD - thread_multiplicar_block_t%d de Matrizes #####\n", real_num);
        start_time = wtime();

        thread_mmultbloco[real_num] = multiplicarMPIBlocos(&mat_a, &mat_b, rank, size);

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
        printf("[mult_t%d vs mult_thread_t%d]\n", i, i);
        mcomparar(mmult[0], thread_mmult[0]);
    }

    for (int i = 0; i < MAX_EXECUTIONS; i++) {
        printf("[mult_t%d vs mult_block_t%d]\n", i, i);
        mcomparar(mmult[0], mmultbloco[0]);
    }

    for (int i = 0; i < MAX_EXECUTIONS; i++) {
        printf("[mult_t%d vs mult_block_thread_t%d]\n", i, i);
        mcomparar(mmult[0], thread_mmultbloco[0]);
    }

    printf("\n######## Tempos Médios (%d execuções)\n", MAX_EXECUTIONS);
    double tempo_normal_seq, tempo_normal_thr, tempo_bloco_seq, tempo_bloco_thr;

    tempo_normal_seq = tempo_normal_thr = tempo_bloco_seq = tempo_bloco_thr = 0;

    for (int i = 0; i < MAX_EXECUTIONS; i++) {
        tempo_normal_seq += seq_normal_times[i];
        tempo_normal_thr += thr_normal_times[i];
        tempo_bloco_seq += seq_bloco_times[i];
        tempo_bloco_thr += thr_bloco_times[i];
    }

    double media_normal_seq = tempo_normal_seq / (float)MAX_EXECUTIONS,
        media_normal_thr = tempo_normal_thr / (float)MAX_EXECUTIONS,
        media_bloco_seq = tempo_bloco_seq / (float)MAX_EXECUTIONS,
        media_bloco_thr = tempo_bloco_thr / (float)MAX_EXECUTIONS;

    printf(
        "\nMultiplicação normal sequencial:\t%02lf\
        \nMultiplicação normal com %d threads:\t%02lf\
        \nMultiplicação em bloco sequencial:\t%02lf\
        \nMultiplicação em bloco com %d threads:\t%02lf\n",
        media_normal_seq,
        size,
        media_normal_thr,
        media_bloco_seq,
        size,
        media_bloco_thr
    );

    double speedup_normal = media_normal_seq / media_normal_thr;
    double speedup_bloco = media_bloco_seq / media_bloco_thr;

    printf("\n######## Speedups\n");

    printf("Multiplicação normal:\t%03lf\nMultiplicação bloco:\t%03lf\n", speedup_normal, speedup_bloco);



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
}
