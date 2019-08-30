#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrizv3.h"

#define N 10
#define M 10


int main(int argc, char *argv[]) {
  mymatriz mat_a;

  mat_a.lin = M;
  mat_a.col = N;

  if (malocar(&mat_a)) { printf ("Error..\n"); }

  mimprimir(&mat_a);

  mzerar(&mat_a);

  mimprimir(&mat_a);

  mgerar(&mat_a, -9999);

  mimprimir(&mat_a);

  mliberar(&mat_a);

  return 0;

}
