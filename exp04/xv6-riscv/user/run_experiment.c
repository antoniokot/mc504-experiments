/*
  Sistemas Operacionais - MC504A_2024S2
  Desenvolvido por: Antônio Hideto Borges Kotsubo (236041)
                    Pedro Sanchez Bitencourt (231133)
*/ 

#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define TICKS_PER_SECOND 10

int main (void) {
  int pipe_fd[2];
  
  if (pipe(pipe_fd) < 0) {
    printf("Erro ao criar o pipe\n");
    exit(1);
  }

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

    run_cpu_bound_experiment(n_cpu_exp, pipe_fd);
    run_io_bound_experiment(n_io_exp, pipe_fd);

    calculate_throughput(20, pipe_fd);
  }

  close(pipe_fd[0]);
  close(pipe_fd[1]);

  return 0;
}

// Função para calcular o throughput com base nos tempos de término
void calculate_throughput(int num_processes, int pipe_fd[2]) {
  uint64 start_time = uptime(); // Tempo inicial do experimento
  uint64 end_time;
  int processes_finished = 0;

  // Lê os tempos de término do pipe
  while (read(pipe_fd[0], &end_time, sizeof(end_time)) > 0) {
    processes_finished++;
  }

  // Tempo total do experimento em ticks
  uint64 total_time = end_time - start_time;
  printf("Total seconds passed in the round: %ld\n", total_time);
  printf("Throughput: ");
  print_fixed_point(processes_finished * TICKS_PER_SECOND / total_time);
  printf("\n");
}

#include "kernel/types.h"
#include "user/user.h"

// Função para imprimir valores de ponto flutuante com três casas decimais
void print_float(int integer_part, int decimal_part) {
  // Garante que a parte decimal seja positiva
  if (decimal_part < 0) {
    decimal_part = -decimal_part;
  }
  printf("%d.%03d", integer_part, decimal_part);
}

// Função para converter um número em ponto fixo para uma representação em "ponto flutuante"
void print_fixed_point(int value) {
  // Multiplica por 1000 para obter três casas decimais
  int integer_part = value / 1000;
  int decimal_part = value % 1000;

  // Ajusta a parte decimal para ser positiva
  if (integer_part < 0 && decimal_part != 0) {
    integer_part -= 1;
    decimal_part = 1000 - decimal_part;
  }

  print_float(integer_part, decimal_part);
}
