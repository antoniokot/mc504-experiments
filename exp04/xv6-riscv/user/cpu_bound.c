#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void run_cpu_bound_experiment(int rounds) {
  for (int i = 0; i < rounds; i++) {
    struct GraphsList* graphsList = createDigraphsList(1);

    struct Node** graph = graphsList->graphs[0];
    int num_vertex = *graphsList->num_vertex[0];

    shortestPath(graph, 0, num_vertex);
  }
}