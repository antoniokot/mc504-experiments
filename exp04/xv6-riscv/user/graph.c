#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <stddef.h>

#define MAX_VERTEX 200
#define MIN_VERTEX 100
#define MAX_EDGES 400
#define MIN_EDGES 50
#define MAX_DISTANCE MAX_VERTEX + 1

int cpu_alloc_time = 0;
int cpu_free_time = 0;
int cpu_access_time = 0;

// struct for nodes
typedef struct Node {
  int destination;
  struct Node* next;
} Node;

// struct for the functions returns
typedef struct GraphsList {
  struct Node*** graphs;
  int *lengths;
} GraphsList;

Node* make_node(int destination) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->destination = destination;
  newNode->next = NULL;

  return newNode;
}

void add_edge(Node* adj_list[], int origin, int destination) {
  Node* newNode = make_node(destination);
  newNode->next = adj_list[origin];
  adj_list[origin] = newNode;
}

int exists_edge(Node* adj_list[], int origin, int destination) {
  Node* temp = adj_list[origin];
  while (temp != NULL) {
    if (temp->destination == destination) {
      return 1;
    }
    temp = temp->next;
  }

  return 0;
}

void create_graph(Node* adj_list[], int num_vertex, int num_edges) {
  int origin = 0;
  int destination = 0;

  int start_access_time = uptime();
  int start_alloc_time = uptime();

  int countEgdes = 0;
  while (countEgdes < num_edges) {
    origin = random_range(0, num_vertex-1);
    destination = random_range(0, num_vertex-1);

    if (origin != destination && !exists_edge(adj_list, origin, destination)) {
      add_edge(adj_list, origin, destination);
      countEgdes++;
    }
  }
  
  int end_alloc_time = uptime();
  cpu_alloc_time += end_alloc_time - start_alloc_time;

  int end_access_time = uptime();
  cpu_access_time += end_access_time - start_access_time;
}

GraphsList* create_graphs_list(int num_digraphs) {
  int start_alloc_time = uptime();
  Node*** list = (Node***)malloc(sizeof(Node**) * num_digraphs);
  int* lengths = (int*)malloc(sizeof(int) * num_digraphs);
  int end_alloc_time = uptime();

  cpu_alloc_time += end_alloc_time - start_alloc_time;

  int start_access_time = uptime();
  for (int i = 0; i < num_digraphs; i++) {
    int num_vertex = random_range(MIN_VERTEX, MAX_VERTEX);
    int num_egdes = random_range(MIN_EDGES, MAX_EDGES);

    start_alloc_time = uptime();
    list[i] = (Node**)malloc(sizeof(Node*) * num_vertex);
    end_alloc_time = uptime();
    lengths[i] = num_vertex;

    cpu_alloc_time += end_alloc_time - start_alloc_time;

    if(list[i] == NULL) {
      printf("Error allocating memory for adj_list.\n");
      exit(1);
    }

    for (int j = 0; j < num_vertex; j++) {
      list[i][j] = NULL;
    }

    create_graph(list[i], num_vertex, num_egdes);
  }

  start_alloc_time = uptime();
  GraphsList* result = (GraphsList*)malloc(sizeof(GraphsList));
  end_alloc_time = uptime();

  result->graphs = list;
  result->lengths = lengths;
  int end_access_time = uptime();

  cpu_alloc_time += end_alloc_time - start_alloc_time;
  cpu_access_time += end_access_time - start_access_time;

  return result;
}

int closest(int distance[], int processed[], int num_vertex) {
  int min_index = -1;
  int min = MAX_DISTANCE;

  for (int i = 0; i < num_vertex; i++) {
    if(!processed[i] && distance[i] <= min) {
      min_index = i;
      min = distance[i];
    }
  }

  return min_index;
}

void relax(int dist[], int proc[], int source, int dest) {
  if (!proc[dest] && dist[dest] > dist[source] + 1) {
    dist[dest] = dist[source] + 1;
  }
}

void shortest_path(Node* graph[], int source, int num_vertex) {
  int dist[num_vertex];
  int processed[num_vertex];

  for (int i = 0; i < num_vertex; i++) {
    dist[i] = MAX_DISTANCE;
    processed[i] = 0;
  }

  dist[source] = 0;

  int start_access_time = uptime();
  for (int i = 0; i < num_vertex; i++) {
    int u = closest(dist, processed, num_vertex);
    processed[u] = 1;

    Node* curr = graph[u];

    while(curr != NULL) {
      int v = curr->destination;

      relax(dist, processed, u, v);
      curr = curr->next;
    }
  }
  int end_access_time = uptime();
  cpu_access_time += end_access_time - start_access_time;
}

void solve_shortest_paths(int num_graphs) {
  GraphsList* graphs = create_graphs_list(num_graphs);
  
  int start_access_time = 0;
  int end_access_time = 0;

  for (int i = 0; i < num_graphs; i++) {
    start_access_time = uptime();
    Node** adj_list = graphs->graphs[i];
    int num_vertex = graphs->lengths[i];
    end_access_time = uptime();

    cpu_access_time += end_access_time - start_access_time;

    shortest_path(adj_list, 0, num_vertex);
  }

  start_access_time = uptime();
  int start_free_time = uptime();
  for (int i = 0; i < num_graphs; i++) {
    Node** graph = graphs->graphs[i];
    int num_vertex = graphs->lengths[i];

    for(int j = 0; j < num_vertex; j++) {
      Node* curr = graph[j];

      while(curr != NULL) {
        Node* copy = curr;
        curr = curr->next;
        free(copy);
      }  
    }
    free(graph);
  }

  free(graphs->graphs);
  free(graphs->lengths);
  free(graphs);

  end_access_time = uptime();
  cpu_access_time += end_access_time - start_access_time;
  int end_free_time = uptime();
  cpu_free_time += end_free_time - start_free_time;

  storemoverhead(cpu_alloc_time, cpu_free_time, cpu_access_time);
}
