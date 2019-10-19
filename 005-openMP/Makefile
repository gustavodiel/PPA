# GNU Makefile
# Revisão: ago/2019

CC = gcc
CCFLAGS = -Wall -O3 -fopenmp
LDFLAGS =
TARGET = main_openmp gmat help

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

main_openmp: main_openmp.c matrizv3.o toolsv3.o matriz-operacoesv3.o
		$(CC) $(CCFLAGS) matriz-operacoesv3.o matrizv3.o toolsv3.o main_openmp.c matriz-operacoes-openmp.c -o $@ $(LDFLAGS)

gmat: matrizv3.o toolsv3.o gera_matrizv3.c
		$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)

help:
	@echo
	@echo
	@echo "####### Exemplo de Execução #######"
	@echo "./main_openmp mat_a3x4.example mat_b4x3.example 4"

clean:
	rm -f *.o *~ $(TARGET) *.map *.result
