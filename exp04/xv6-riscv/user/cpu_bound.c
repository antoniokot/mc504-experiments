#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <stddef.h>

void run_cpu_bound_experiment() {
  int pid = fork();
  if (pid < 0) {
    printf("Fork failed\n");
    exit(1);
  } else if (pid == 0) {
    solve_shortest_paths(1000);
    exit(0);
  }
} 