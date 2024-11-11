#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <limits.h>

#define SCALE 1000
#define NUMBER_OF_PROCESSES 20
#define NUMBER_OF_ROUNDS 30
#define TICKS_PER_SECOND 10

int avg_perfomance = 0;

// Função para imprimir valores de ponto flutuante com três casas decimais
void print_float(int integer_part, int decimal_part) {
  // Garante que a parte decimal seja positiva
  if (decimal_part < 0) {
    decimal_part = -decimal_part;
  }

  // Imprime a parte inteira
  printf("%d.", integer_part);

  // Adiciona zeros à esquerda conforme necessário para a parte decimal
  if (decimal_part < 10) {
    printf("00%d", decimal_part);
  } else if (decimal_part < 100) {
    printf("0%d", decimal_part);
  } else {
    printf("%d", decimal_part);
  }

  printf("\n");
}

// Função para converter um número em ponto fixo para uma representação em "ponto flutuante"
void print_fixed_point(int value) {
  int integer_part = value / SCALE;
  int decimal_part = value % SCALE;

  // Ajusta a parte decimal para ser positiva
  if (integer_part < 0 && decimal_part != 0) {
    integer_part -= 1;
    decimal_part = SCALE - decimal_part;
  }

  print_float(integer_part, decimal_part);
}

// Função para calcular o throughput com base nos tempos de término
void calculate_throughput() {
  int t_put_norm = throughput();

  avg_perfomance += t_put_norm;

  printf("Throughput normalizado (T_put_norm): ");
  print_fixed_point(t_put_norm);
}

void register_read_duration(uint64 duration, struct file_efficiency_metrics* file_efficiency_metrics) {
  file_efficiency_metrics->file_read_duration[file_efficiency_metrics->file_read_count++] = duration;
}
void register_write_duration(uint64 duration, struct file_efficiency_metrics* file_efficiency_metrics) {
  file_efficiency_metrics->file_write_duration[file_efficiency_metrics->file_write_count++] = duration;
}
void register_delete_duration(uint64 duration, struct file_efficiency_metrics* file_efficiency_metrics) {
  file_efficiency_metrics->file_delete_duration[file_efficiency_metrics->file_delete_count++] = duration;
}

void print_padded_int(uint64 num, int width) {
  char buffer[20];
  int length = 0;
  uint64 temp = num;
  do {
    temp /= 10;
    length++;
  } while (temp != 0);
  int padding = width - length;
  if (padding > 0) {
    for (int i = 0; i < padding; i++) {
      buffer[i] = '0';
    }
  }
  int index = padding > 0 ? padding : 0;
  temp = num;
  do {
    buffer[index + length - 1] = '0' + (temp % 10);
    temp /= 10;
    length--;
  } while (length > 0);

  buffer[width] = '\0';
  printf("%s", buffer);
}

void calculate_normalized_fs_efficiency(uint64 fs_efficiency, uint64 max_efficiency, uint64 min_efficiency) {
  if (max_efficiency != min_efficiency) { // Evita divisão por zero
    int fs_efficiency_norm = SCALE - ((fs_efficiency - min_efficiency) * SCALE / (max_efficiency - min_efficiency));
    
    avg_perfomance += fs_efficiency_norm;

    printf("Eficiência do sistema de arquivos normalizada (E_fs_norm): %d.", fs_efficiency_norm / SCALE);
    print_padded_int(fs_efficiency_norm % 1000, 3);
    printf("\n");
  } else {
    printf("Eficiência do sistema de arquivos (E_fs_norm): 1.000\n");
  }
}

void calculate_file_efficiency(int fs_pipe_fd[2]) {
  int total_processes = 0;
  uint64 total_efficiency = 0;
  uint64 max_efficiency = 0;
  uint64 min_efficiency = 2147483647;
  uint64 partial_fs_efficiency;
  while(read(fs_pipe_fd[0], &partial_fs_efficiency, sizeof(partial_fs_efficiency)) > 0) {
    total_efficiency += partial_fs_efficiency;
    if (partial_fs_efficiency > max_efficiency) max_efficiency = partial_fs_efficiency;
    if (partial_fs_efficiency < min_efficiency) min_efficiency = partial_fs_efficiency;
    total_processes++;
  }

  uint64 fs_efficiency = total_efficiency / total_processes;

  calculate_normalized_fs_efficiency(fs_efficiency, max_efficiency, min_efficiency);
}

void calculate_fairness() {
  int j_cpu = fairness();

  avg_perfomance += j_cpu;

  printf("Justiça entre processos (J_cpu): ");
  print_fixed_point(j_cpu);
}

void calculate_memory_overhead() {
  int m_over_norm = moverhead();

  avg_perfomance += m_over_norm;

  printf("Overhead de memoria normalizado (M_over_norm): ");
  print_fixed_point(m_over_norm);
}

void calculate_average_system_performance() {
  int final_avg_system_performance = avg_perfomance / 4;

  printf("Performance média do sistema (S_perform): ");
  print_fixed_point(final_avg_system_performance);
}

void get_metrics(int fs_pipe_fd[2]) {
  printf("\nMetrics:\n\n");

  printf("- ");
  calculate_throughput();
  printf("- ");
  calculate_fairness();
  printf("- ");
  calculate_file_efficiency(fs_pipe_fd);
  printf("- ");
  calculate_memory_overhead();
  printf("\n-----------------------------------------\n");
  printf("\n- ");
  calculate_average_system_performance();

  avg_perfomance = 0;
}