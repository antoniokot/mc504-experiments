/*
  Sistemas Operacionais - MC504A_2024S2
  Desenvolvido por: Antônio Hideto Borges Kotsubo (236041)
                    Pedro Sanchez Bitencourt (231133)
*/ 

#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void run_experiment(int num_cpu, int num_io, int fs[2]) {
  int n_cpu_exp = num_cpu;
  int n_io_exp = num_io;

  while(n_cpu_exp + n_io_exp > 0) {
    if (n_cpu_exp > 0) {
      run_cpu_bound_experiment();
      n_cpu_exp--;
    }
    if (n_io_exp > 0) {
      run_io_bound_experiment(fs, num_io - n_io_exp);
      n_io_exp--;
    }
  }
}

void wait_all_processes(int num_processes) {
  int finished_processes = 0;
  while(finished_processes < num_processes) {
    int pid = getpid();
    wait(&pid);
    finished_processes++;
  }
}

int main (void) {
  int fs_pipe_fd[2];
  
  if (pipe(fs_pipe_fd) < 0) {
    printf("Erro ao criar o pipe\n");
    exit(1);
  }

  printf("\nRunning 30 rounds of experiments...\n");

  for(int i =  0; i < 30; i++) {
    printf("\n======================================\n");
    printf("\nRunning round number %d... \n\n", i+1);

    int n_cpu_exp = random(8) + 6;
    int n_io_exp = 20 - n_cpu_exp;

    printf("Number of CPU-Bound processes: %d\n", n_cpu_exp);
    printf("Number of IO-Bound processes: %d\n", n_io_exp);

    run_experiment(n_cpu_exp, n_io_exp, fs_pipe_fd);

    wait_all_processes(20);

    close(fs_pipe_fd[1]);

    get_metrics(fs_pipe_fd);

    close(fs_pipe_fd[0]);

    if (pipe(fs_pipe_fd) < 0) {
      printf("Erro ao recriar o pipe\n");
      exit(1);
    }
  }
  close(fs_pipe_fd[0]);
  close(fs_pipe_fd[1]);

  return 0;
}
