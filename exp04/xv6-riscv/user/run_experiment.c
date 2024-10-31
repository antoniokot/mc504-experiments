/*
  Sistemas Operacionais - MC504A_2024S2
  Desenvolvido por: Antônio Hideto Borges Kotsubo (236041)
                    Pedro Sanchez Bitencourt (231133)
*/ 

#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main (void) {
  int pipe_fd[2];
  
  if (pipe(pipe_fd) < 0) {
    printf("Erro ao criar o pipe\n");
    exit(1);
  }

  printf("Running 30 rounds of experiments...\n");

  for(int i =  0; i < 30; i++) {
    printf("\n======================================\n");
    printf("\nRunning round number %d... \n\n", i+1);
    int start_time = uptime();

    int n_cpu_exp = random(8) + 6;      // random int from 6 to 14
    int n_io_exp = 20 - n_cpu_exp;

    printf("Number of CPU-Bound processes: %d\n", n_cpu_exp);
    printf("Number of IO-Bound processes: %d\n", n_io_exp);

    run_cpu_bound_experiment(n_cpu_exp, pipe_fd);
    run_io_bound_experiment(n_io_exp, pipe_fd);

    close(pipe_fd[1]);

    get_metrics(start_time, pipe_fd);

    printf("\n======================================\n");

    if (pipe(pipe_fd) < 0) {
      printf("Erro ao recriar o pipe\n");
      exit(1);
    }
  }

  close(pipe_fd[0]);
  close(pipe_fd[1]);

  return 0;
}
