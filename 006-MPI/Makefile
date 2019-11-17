# GNU Makefile
# Revisão: ago/2019

CC = mpicc
CCFLAGS = -Wall -O3 -fopenmp
LDFLAGS =
TARGET = main_mpi gmat help

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

main_mpi: main_mpi.c matrizv3.o toolsv3.o matriz-operacoesv3.o
		$(CC) $(CCFLAGS) matriz-operacoesv3.o matrizv3.o toolsv3.o main_mpi.c matriz-operacoes-mpi.c -o $@ $(LDFLAGS)

gmat: matrizv3.o toolsv3.o gera_matrizv3.c
		$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)

help:
	@echo
	@echo
	@echo "####### Exemplo de Execução #######"
	@echo "./main_mpi mat_a3x4.example mat_b4x3.example 4"

clean:
	rm -f *.o *~ $(TARGET) *.map *.result
