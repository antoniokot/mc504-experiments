#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <stddef.h>

#define MAX_VERTEX 20
#define MIN_VERTEX 10
#define MAX_EDGES 40
#define MIN_EDGES 5
#define MAX_DISTANCE MAX_VERTEX + 1

// struct for nodes
typedef struct Node {
  int destination;
  struct Node* next;
} Node;

typedef struct GraphsList {
  struct Node*** graphs;
  int** num_vertex;
} GraphsList;

// function to create a new node 
Node* makeNode(int destination) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->destination = destination;
  newNode->next = NULL;
  return newNode;
}

/**
 * @brief Adds a new edge (connection) to the graph.
 *
 * This function creates a new node with the given destination and adds it to the adjacency list of the origin node.
 * If the adjacency list of the origin node is empty, the new node becomes the head of the list. Otherwise, the new node is
 * inserted at the beginning of the list.
 *
 * @param listaAdj The adjacency list of the graph. It is an array of pointers to Node structures, where each pointer
 * represents the head of a linked list of adjacent nodes for a specific vertex.
 * @param origin The index of the origin vertex.
 * @param destination The index of the destination vertex.
 */
void addEdge(Node* adjList[], int origin, int destination) {
  Node* newNode = makeNode(destination);
  newNode->next = adjList[origin];
  adjList[origin] = newNode;
}

// Função para verificar se uma aresta já existe
int existsEdge(Node* adjList[], int origin, int destination) {
  Node* temp = adjList[origin];
  while (temp != NULL) {
    if (temp->destination == destination) {
      return 1; // edge already exists
    }
    temp = temp->next;
  } 
  return 0;
}

// Função para gerar um dígrafo aleatório
void createDigraph(Node* adjList[], int num_vertex, int num_edge) {
  int origin, destination;

  // Initialize the adjacency list with NULL
  for (int i = 0; i < num_vertex; i++) {
    adjList[i] = NULL;
  }

  // Create random edges
  int countEgdes = 0;
  while (countEgdes < num_edge) {
    origin = random(num_vertex);
    destination = random(num_edge);

    // Not loop edge and egde does not already exist 
    if (origin != destination && !existsEdge(adjList, origin, destination)) {
      addEdge(adjList, origin, destination); // add edge
      countEgdes++;
    }
  }
}

void imprimirDigrafo(Node* listaAdj[], int vertices) {
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

GraphsList* createDigraphsList(int num_digraphs) {
  Node*** digraphsList = (Node***)malloc(sizeof(Node**) * num_digraphs);
  int** lengths = (int**)malloc(sizeof(int) * num_digraphs);

  for (int i = 0; i < num_digraphs; i++) {
    int num_vertex = (random(MAX_VERTEX - MIN_VERTEX + 1)) + MIN_VERTEX;
    int num_egdes = (random(MAX_EDGES - MIN_EDGES + 1)) + MIN_EDGES;

    printf("Número de vértices: %d\n", num_vertex);
    printf("Número de arestas: %d\n", num_egdes);

    Node** adjList = (Node**)malloc(sizeof(Node*) * num_vertex);

    createDigraph(adjList, num_vertex, num_egdes);
    *lengths[i] = num_vertex;

    digraphsList[i] = adjList;
    imprimirDigrafo(digraphsList[0], num_vertex);
  }

  GraphsList* list = (GraphsList*)malloc(sizeof(GraphsList));

  list->graphs = digraphsList;
  list->num_vertex = lengths;

  return list;
}

int closest(int distance[], int processed[], int num_vertex) {
  int min_index = -1;
  int min = MAX_DISTANCE;

  for (int i = 0; i < num_vertex; i++) {
    if(!processed[i] && distance[i] < min) {
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

void shortestPath(Node* graph[], int source, int num_vertex) {
  int dist[num_vertex];
  int processed[num_vertex];

  for (int i = 0; i < num_vertex; i++) {
    dist[i] = MAX_DISTANCE;
    processed[i] = 0;
  }

  dist[source] = 0;

  for (int i = 0; i < num_vertex - 1; i++) {
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

  for (int i = 0; i < num_vertex; i++) {
    printf("Distância do vértice %d até o vértice %d: %d\n", source, i, dist[i]);
  }
}
