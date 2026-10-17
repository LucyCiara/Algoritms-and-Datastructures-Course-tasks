#include <stdio.h>
#include <stdlib.h>

typedef struct wEdge {
  struct wEdge *next;
  struct node *to;
  int weight;
} WEdge;

typedef struct node {
  WEdge *edge;
  void *d;
} Node;

typedef struct {
  int N, E;
  Node *node;
} Graph;

Graph *newGraph(FILE *f) {
  Graph *G = (Graph *)malloc(sizeof(Graph));
  fscanf(f, "%i %i\n", &G->N, &G->E);
  G->node = (Node *)calloc(G->N, sizeof(Node));
  for (int i = 0; i < G->E; ++i) {
    int from, to;
    WEdge *e = (WEdge *)malloc(sizeof(WEdge));
    fscanf(f, "%i %i %i\n", &from, &to, &e->weight);
    e->to = &G->node[to];
    e->next = G->node[from].edge;
    G->node[from].edge = e;
  }
  return G;
}
