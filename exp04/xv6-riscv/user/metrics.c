#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define TICKS_PER_SECOND 10

int T_put_max = 0;
int T_put_min = __INT_MAX__;

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

void 
calculate_normalized_throughput(int throughput) {
  if (T_put_max != T_put_min) { // Evita divisão por zero
    int T_put_norm = 1000 - ((throughput - T_put_min) * 1000 / (T_put_max - T_put_min));
    
    printf("Normalized Throughput: ");
    print_fixed_point(T_put_norm);
  } else {
    printf("Normalized Throughput: 1.000\n"); // Caso T_put_max == T_put_min
  }
}

// Função para calcular o throughput com base nos tempos de término
void calculate_throughput(int start_time, int pipe_fd[2]) {
  int end_time;
  int processes_finished = 0;

  // Lê os tempos de término do pipe
  while (read(pipe_fd[0], &end_time, sizeof(end_time)) > 0) {
    processes_finished++;
  }

  // Tempo total do experimento em ticks
  int total_time = end_time - start_time; 
  int throughput = processes_finished * TICKS_PER_SECOND / total_time;

  // Atualize T_put_max e T_put_min
  if (throughput > T_put_max) {
    T_put_max = throughput;
  }

  if (throughput < T_put_min) {
    T_put_min = throughput;
  }

  printf("Total seconds passed in the round: %d\n", total_time / TICKS_PER_SECOND);
  calculate_normalized_throughput(throughput);
}

void get_metrics(int start_time, int pipe_fd[2]) {
  printf("\nMetrics:\n");

  calculate_throughput(start_time, pipe_fd);
}