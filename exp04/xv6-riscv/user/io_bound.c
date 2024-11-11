#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

uint64 get_file_system_efficiency(struct file_efficiency_metrics* file_efficiency_metrics) {
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
  
  return read_efficiency + write_efficiency + delete_efficiency;
}

void run_io_bound_experiment(int num_process, int fs_pipe_fd[2]) {
  printf("\nRunning IO-Bound processes...\n");

  for(int i = 0; i < num_process; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("Fork failed");
      exit(1);
    } else if (pid == 0) {
      struct file_efficiency_metrics* file_efficiency_metrics = (struct file_efficiency_metrics*) malloc(sizeof(struct file_efficiency_metrics));
      random_write(fs_pipe_fd, file_efficiency_metrics);

      uint64 fs_efficiency = get_file_system_efficiency(file_efficiency_metrics);
      write(fs_pipe_fd[1], &fs_efficiency, sizeof(fs_efficiency));
      free(file_efficiency_metrics);

      exit(0);
    } else {
      int retPid = wait(&pid);
      kill(retPid);
    }
  }
  printf("Finished IO-Bound processes\n");
}