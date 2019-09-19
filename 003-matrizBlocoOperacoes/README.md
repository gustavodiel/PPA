
# Disciplina de Programação Paralela Avançada

Revisão: set/2019
Linguagem: C
Versão: v3 (tools, matriz, matriz-operacoes)
Executáveis: mainEx02 e gmat


PS: Para ver melhor esse arquivo, sugiro abrir no Github: https://github.com/gustavodiel/PPA/blob/master/002-matrizOperacoes/README.md


## Compilar

Digitar no terminal:
```sh
make clean
make
```


## Executar

Digitar no terminal:
```sh
./mainEx02 ARQV_MATRIZ_A ARQV_MATRIZ_B
```

Por exemplo:
```sh
./mainEx02 mat_a3x4.example mat_b4x3.example
```

ARQUIVOS:
 * README-res: resultado da execução de exemplo (./main mat_a3x4.example mat_b4x3.example);
 * mat_a3x4.example e mat_b4x3.example: matrizes exemplos A (3x4) e B(4x3);
 * toolsv3.(c/h): biblioteca de leitura/escrita de matrizes em arquivos;
 * matriz-operacoesv3.h: headers das funções de operações de matrizes que devem ser implementadas;
 * matriz-operacoesv3.c: Implementado por Gustavo Diel
 * matrizv3.h: headers das funções de gerência de matrizes que devem ser implementadas;
 * matrizv3.c: Implementado por Gustavo Diel
 * gera_matrizv3.c: fontes do programa de geração de matrizes em arquivo;
 * main_matrizv3.c: fontes do programa de teste das operações de matrizes;
 * .gitignore: Arquivo para deixar o repositório do Git limpo (pode ignorar)
