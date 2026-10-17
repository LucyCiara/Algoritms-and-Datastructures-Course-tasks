#include <stdio.h>
#include <stdlib.h>

typedef struct wEdge {
  struct wEdge *next;
  struct wEdge *opposite;
  struct node *to;
  int capacity;
  int flow;
} WEdge;

typedef struct node {
  WEdge *edgeDown;
  WEdge *edgeUp;
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
    WEdge *re = (WEdge *)calloc(1, sizeof(WEdge));
    fscanf(f, "%i %i %i\n", &from, &to, &e->capacity);
    e->to = &G->node[to];
    e->next = G->node[from].edgeDown;
    re->to = &G->node[from];
    re->capacity = 0;
    re->opposite = e;
    e->opposite = re;
    G->node[from].edgeDown = e;
    G->node[from].edgeUp = re;
  }
  return G;
}
