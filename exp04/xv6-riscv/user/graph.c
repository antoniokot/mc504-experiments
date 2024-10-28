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
      return 1; // edge already exists
    }
    temp = temp->next;
  } 
  return 0;
}

void create_graph(Node* adj_list[], int num_vertex, int num_edges) {
  int origin = 0;
  int destination = 0;

  // Create random edges
  int countEgdes = 0;
  while (countEgdes < num_edges) {
    origin = random_range(0, num_vertex-1);
    destination = random_range(0, num_vertex-1);

    // Not loop edge and egde does not already exist 
    if (origin != destination && !exists_edge(adj_list, origin, destination)) {
      add_edge(adj_list, origin, destination); // add edge
      countEgdes++;
    }
  }
}

void print_graph(Node* listaAdj[], int vertices) {
  for (int i = 0; i < vertices; i++) {
    printf("Vértice %d:", i);

    Node* temp = listaAdj[i];
    while (temp != NULL) {
      printf(" -> %d", temp->destination);
      temp = temp->next;
    }
    printf("\n");
  }
}

GraphsList* create_graphs_list(int num_digraphs) {
  Node*** list = (Node***)malloc(sizeof(Node**) * num_digraphs);
  int* lengths = (int*)malloc(sizeof(int) * num_digraphs);

  printf("Creating %d graph(s)...\n", num_digraphs);

  for (int i = 0; i < num_digraphs; i++) {
    int num_vertex = random_range(MIN_VERTEX, MAX_VERTEX); // 100 - 200 vértices -> 150 vértices [0, 150]
    int num_egdes = random_range(MIN_EDGES, MAX_EDGES);

    list[i] = (Node**)malloc(sizeof(Node*) * num_vertex);
    lengths[i] = num_vertex;

    if(list[i] == NULL) {
      printf("Error allocating memory for adj_list.\n");
      exit(1);
    }

    // Initialize the adjacency list with NULL
    for (int j = 0; j < num_vertex; j++) {
      list[i][j] = NULL;
    }

    create_graph(list[i], num_vertex, num_egdes);
  }

  GraphsList* result = (GraphsList*)malloc(sizeof(GraphsList));

  result->graphs = list;
  result->lengths = lengths;

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

  for (int i = 0; i < num_vertex; i++) {
    int u = closest(dist, processed, num_vertex);
    processed[u] = 1;

    // adjacency list of u
    Node* curr = graph[u];

    while(curr != NULL) {
      int v = curr->destination;

      relax(dist, processed, u, v);
      curr = curr->next;
    }  
  }
}

void solve_shortest_paths(int num_graphs) {
  GraphsList* graphs = create_graphs_list(num_graphs);
  
  printf("Solving shortest paths...\n");
  for (int i = 0; i < num_graphs; i++) {
    Node** adj_list = graphs->graphs[i];
    int num_vertex = graphs->lengths[i];

    shortest_path(adj_list, 0, num_vertex);
  }
  printf("Shortest paths solved.\n");

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
}
