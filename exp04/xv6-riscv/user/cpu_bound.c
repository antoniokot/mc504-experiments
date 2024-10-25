#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <stddef.h>

void run_cpu_bound_experiment(int rounds) {
  printf("\nRunning CPU-bound experiment with %d rounds...\n", rounds);

  for (int i = 0; i < rounds; i++) {
    printf("\n=============================================\n");
    printf("\nRunning round %d...\n", i+1);

    int pid = fork();
    
    if(pid == 0) {
      solve_shortest_paths(1000);
    } else {
      int ret = wait(&pid);  // wait for the child process to finish
      kill(ret);
    }
  }
}