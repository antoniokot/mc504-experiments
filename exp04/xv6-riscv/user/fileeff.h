#ifndef FSEFFICIENCYSTRUCT_H
#define FSEFFICIENCYSTRUCT_H
#include "kernel/types.h"
#define MAX_CALLS 1000
#define MAX_PROCESSES 14

struct file_efficiency_metrics {
  uint64 file_write_duration[MAX_PROCESSES][MAX_CALLS];
  int file_write_count;
  uint64 file_read_duration[MAX_PROCESSES][MAX_CALLS];
  int file_read_count;
  uint64 file_delete_duration[MAX_PROCESSES][MAX_CALLS];
  int file_delete_count;
};

extern struct file_efficiency_metrics file_efficiency_metrics;

#endif