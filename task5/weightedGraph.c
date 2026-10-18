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
  WEdge *edge;
  int num;
  void *d;
} Node;

typedef struct {
  int N, E;
  Node *node;
} Graph;

int connectReverse(Node *n, WEdge *e) {
  Node *rn = e->to;
  if (rn->edge) {
    for (WEdge *re = rn->edge; re; re = re->next) {
      if (re->to == n && !re->opposite) {
        e->opposite = re;
        re->opposite = e;
        return 1;
      }
    }
  }

  return 0;
}

Graph *newGraph(FILE *f) {
  Graph *G = (Graph *)malloc(sizeof(Graph));
  fscanf(f, "%i %i\n", &G->N, &G->E);
  G->node = (Node *)calloc(G->N, sizeof(Node));
  for (int i = 0; i < G->E; ++i) {
    int from, to;
    WEdge *e = (WEdge *)calloc(1, sizeof(WEdge));
    fscanf(f, "%i %i %i\n", &from, &to, &e->capacity);
    G->node[from].num = from;
    G->node[to].num = to;
    e->to = &G->node[to];
    e->next = G->node[from].edge;
    G->node[from].edge = e;
  }

  for (int i = 0; i < G->N; ++i) {
    Node *n = &G->node[i];
    for (WEdge *e = n->edge; e; e = e->next) {
      if (!e->opposite && !connectReverse(n, e)) {
        if (e->to->edge) {
          WEdge *ie;
          for (ie = e->to->edge; ie->next != NULL; ie = ie->next)
            ;
          ie->next = calloc(1, sizeof(WEdge));
          ie->next->to = n;
          ie->next->capacity = 0;
          ie->next->opposite = e;
          e->opposite = ie->next;
        } else {
          e->to->edge = calloc(1, sizeof(WEdge));
          e->to->edge->to = n;
          e->to->edge->capacity = 0;
          e->to->edge->opposite = e;
          e->opposite = e->to->edge;
        }
      }
    }
  }

  return G;
}
