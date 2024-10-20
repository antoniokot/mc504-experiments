#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void run_cpu_bound_experiment() {
  struct Node*** digraphsList = createDigraphsList(1);

  for(int i = 0; i < 1; i++) {  
    int size = shortestPath(digraphsList[0]);
    printf("Total: %d\n", size);
  }
}