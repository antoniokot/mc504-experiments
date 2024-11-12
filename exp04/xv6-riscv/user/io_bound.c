#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void get_file_system_efficiency(struct file_efficiency_metrics* file_efficiency_metrics, uint64* fs_efficiencies) {
  uint64 total_read_duration = 0;
  uint64 total_write_duration = 0;
  uint64 total_delete_duration = 0;

  for (int j = 0; j < file_efficiency_metrics->file_read_count; j++) {
    total_read_duration += file_efficiency_metrics->file_read_duration[j] * 1000;
  }
  for (int j = 0; j < file_efficiency_metrics->file_write_count; j++) {
    total_write_duration += file_efficiency_metrics->file_write_duration[j] * 1000;
  }
  for (int j = 0; j < file_efficiency_metrics->file_delete_count; j++) {
    total_delete_duration += file_efficiency_metrics->file_delete_duration[j] * 1000;
  }

  uint64 read_efficiency = total_read_duration / file_efficiency_metrics->file_read_count;
  uint64 write_efficiency = total_write_duration / file_efficiency_metrics->file_write_count;
  uint64 delete_efficiency = total_delete_duration / file_efficiency_metrics->file_delete_count;
  
  fs_efficiencies[0] = read_efficiency;
  fs_efficiencies[1] = write_efficiency;
  fs_efficiencies[2] = delete_efficiency;
}

void run_io_bound_experiment(int fs_pipe_fd[2]) {
  int pid = fork();
  if (pid < 0) {
    printf("Fork failed");
    exit(1);
  } else if (pid == 0) {
    struct file_efficiency_metrics* file_efficiency_metrics = (struct file_efficiency_metrics*) malloc(sizeof(struct file_efficiency_metrics));
    random_write(fs_pipe_fd, file_efficiency_metrics);

    uint64 fs_efficiencies[3];
    get_file_system_efficiency(file_efficiency_metrics, fs_efficiencies);
    write(fs_pipe_fd[1], fs_efficiencies, sizeof(fs_efficiencies));
    free(file_efficiency_metrics);

    exit(0);
  }
}