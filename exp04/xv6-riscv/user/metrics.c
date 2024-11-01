#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "user/fileeff.h"

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

void calculate_file_efficiency(int total_processes) {
  uint64 total_efficiency = 0;
  uint64 max_efficiency = 0;
  uint64 min_efficiency = 2147483647;

  for (int i = 0; i < total_processes; i++) {
    uint64 total_read_duration = 0;
    uint64 total_write_duration = 0;
    uint64 total_delete_duration = 0;

    for (int j = 0; j < file_efficiency_metrics.file_read_count; i++) {
      total_read_duration += file_efficiency_metrics.file_read_duration[i][j];
    }
    for (int j = 0; j < file_efficiency_metrics.file_write_count; i++) {
      total_write_duration += file_efficiency_metrics.file_write_duration[i][j];
    }
    for (int j = 0; j < file_efficiency_metrics.file_delete_count; i++) {
      total_delete_duration += file_efficiency_metrics.file_delete_duration[i][j];
    }

    uint64 read_efficiency = total_read_duration / file_efficiency_metrics.file_read_count;
    uint64 write_efficiency = total_write_duration / file_efficiency_metrics.file_write_count;
    uint64 delete_efficiency = total_delete_duration / file_efficiency_metrics.file_delete_count;

    uint64 partial_fs_efficiency = read_efficiency + write_efficiency + delete_efficiency;
    total_efficiency += partial_fs_efficiency;
    if (partial_fs_efficiency > max_efficiency) max_efficiency = partial_fs_efficiency;
    if (partial_fs_efficiency < min_efficiency) min_efficiency = partial_fs_efficiency;
  }

  uint fs_efficiency = total_efficiency / total_processes;
  printf("File System Efficiency: %d seconds\n", fs_efficiency / TICKS_PER_SECOND);

  for (int i = 0; i < total_processes; i++) {
    memset(file_efficiency_metrics.file_read_duration[i], 0, sizeof(file_efficiency_metrics.file_read_duration[i]));
    memset(file_efficiency_metrics.file_write_duration[i], 0, sizeof(file_efficiency_metrics.file_write_duration[i]));
    memset(file_efficiency_metrics.file_delete_duration[i], 0, sizeof(file_efficiency_metrics.file_delete_duration[i]));
  }
}

void get_metrics(int start_time, int n_io_processes, int pipe_fd[2]) {
  printf("\nMetrics:\n");

  calculate_throughput(start_time, pipe_fd);
  calculate_file_efficiency(n_io_processes);
}