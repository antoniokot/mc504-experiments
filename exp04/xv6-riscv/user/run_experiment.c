/*
  Sistemas Operacionais - MC504A_2024S2
  Desenvolvido por: Antônio Hideto Borges Kotsubo (236041)
*/ 

#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "stdlib.h"
#include "graph.h"

int main (void) {

  int n_cpu_exp = rand() % 8 + 6;      // random int from 6 to 14
  int n_io_exp = 20 - n_cpu_exp;

  printf("Rounds of CPU-Bound experiments: %d\n", n_cpu_exp);
  printf("Rounds of IO-Bound experiments: %d\n", n_io_exp);

  int sum = 0;

  for (int i = n_cpu_exp; i > 0; i--) {
    sum += shortestPath();
  }

  printf("Result: %d\n", sum);

  return 0;
}