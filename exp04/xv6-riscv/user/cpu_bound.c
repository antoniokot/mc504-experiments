#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void run_cpu_bound_experiment(int rounds) {
  for (int i = 0; i < rounds; i++) {
    struct Node*** digraphsList = createDigraphsList(1);

    shortestPath(digraphsList[0], 0);
  }
}