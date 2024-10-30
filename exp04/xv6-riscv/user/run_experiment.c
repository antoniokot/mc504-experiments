/*
  Sistemas Operacionais - MC504A_2024S2
  Desenvolvido por: Antônio Hideto Borges Kotsubo (236041)
                    Pedro Sanchez Bitencourt (231133)
*/ 

#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main (void) {
  printf("Running 30 rounds of experiments...\n");

  for(int i =  0; i < 30; i++) {
    start_round();
    printf("\n======================================\n");
    printf("\nRunning round number %d... \n\n", i+1);

    int n_cpu_exp = random(8) + 6;      // random int from 6 to 14
    int n_io_exp = 20 - n_cpu_exp;

    printf("Number of CPU-Bound processes: %d\n", n_cpu_exp);
    printf("Number of IO-Bound processes: %d\n", n_io_exp);

    printf("\n======================================\n");

    run_cpu_bound_experiment(n_cpu_exp);
    run_io_bound_experiment(n_io_exp);

    calculate_throughput();
  }

  return 0;
}