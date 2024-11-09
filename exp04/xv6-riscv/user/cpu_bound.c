#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <stddef.h>

void run_cpu_bound_experiment(int num_process) {
  printf("\nRunning CPU-Bound processes...\n");

  for (int i = 0; i < num_process; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("Fork failed\n");
      exit(1);
    } else if (pid == 0) {
      solve_shortest_paths(1000); // Trabalho CPU-bound simulado
      mem_overhead_count++;
      incremoverhcount();
      exit(0);
    } else {
      int retPid = wait(&pid);
      kill(retPid);
    }
  }

  printf("Finished CPU-Bound processes\n");
} 