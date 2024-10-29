#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <stddef.h>

void run_cpu_bound_experiment(int num_process) {
  printf("\nRunning CPU-Bound processes...\n");

  for (int i = 0; i < num_process; i++) {
    printf("\nRunning process number %d...\n", i+1);

    int pid = fork();
    
    if (pid < 0) {
      printf("Fork failed");
      exit(1);
    } else if(pid == 0) {
      solve_shortest_paths(1000);
      exit(0);
    } else {
      int ret = wait(&pid);  // wait for the child process to finish
      kill(ret);
    }
  }
}