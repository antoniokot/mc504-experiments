#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void get_file_system_efficiency(struct file_efficiency_metrics* file_efficiency_metrics, uint64* fs_efficiencies) {
  uint64 total_read_duration = 0;
  uint64 total_write_duration = 0;
  uint64 total_delete_duration = 0;

  for (int j = 0; j < file_efficiency_metrics->file_read_count; j++) {
    total_read_duration += file_efficiency_metrics->file_read_duration[j] * SCALE;
  }
  for (int j = 0; j < file_efficiency_metrics->file_write_count; j++) {
    total_write_duration += file_efficiency_metrics->file_write_duration[j] * SCALE;
  }
  for (int j = 0; j < file_efficiency_metrics->file_delete_count; j++) {
    total_delete_duration += file_efficiency_metrics->file_delete_duration[j] * SCALE;
  }

  fs_efficiencies[0] = total_read_duration;
  fs_efficiencies[1] = total_write_duration;
  fs_efficiencies[2] = total_delete_duration;
}

void run_io_bound_experiment(int fs_pipe_fd[2], int i_process) {
  int pid = fork();
  if (pid < 0) {
    printf("Fork failed");
    exit(1);
  } else if (pid == 0) {
    struct file_efficiency_metrics* file_efficiency_metrics = (struct file_efficiency_metrics*) malloc(sizeof(struct file_efficiency_metrics));
    random_write(fs_pipe_fd, i_process, file_efficiency_metrics);

    uint64 fs_efficiencies[3];
    get_file_system_efficiency(file_efficiency_metrics, fs_efficiencies);
    write(fs_pipe_fd[1], fs_efficiencies, sizeof(fs_efficiencies));
    free(file_efficiency_metrics);

    exit(0);
  }
}