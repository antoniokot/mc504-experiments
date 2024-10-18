#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <stddef.h>

#define MAX_VERTEX 200
#define MIN_VERTEX 100
#define MAX_EDGES 400
#define MIN_EDGES 50

// struct for nodes
typedef struct Node {
  int destination;
  struct Node* next;
} Node;

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

Node*** createDigraphsList(int num_digraphs) {
  Node*** digraphsList = NULL;

  for (int i = 0; i < num_digraphs; i++) {
    int num_vertex = (random(MAX_VERTEX - MIN_VERTEX + 1)) + MIN_VERTEX;
    int num_egdes = (random(MAX_EDGES - MIN_EDGES + 1)) + MIN_EDGES;

    digraphsList = (Node***)malloc((sizeof(Node) * num_vertex) * num_digraphs);

    Node** adjList = (Node**)malloc(sizeof(Node) * num_vertex);

    createDigraph(adjList, num_vertex, num_egdes);

    digraphsList[i] = adjList;
  }

  return digraphsList;
}

int shortestPath() {
  return 1;
}
