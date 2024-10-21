/*
  Sistemas Operacionais - MC504A_2024S2
  Desenvolvido por: Antônio Hideto Borges Kotsubo (236041)
                    Pedro Sanchez Bitencourt (231133)
*/ 

#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main (void) {
  int n_cpu_exp = random(8) + 6;      // random int from 6 to 14
  int n_io_exp = 20 - n_cpu_exp;

  printf("Rounds of CPU-Bound experiments: %d\n", n_cpu_exp);
  printf("Rounds of IO-Bound experiments: %d\n", n_io_exp);

  // run_cpu_bound_experiment(1);
  run_io_bound_experiment(n_io_exp);

  return 0;
}