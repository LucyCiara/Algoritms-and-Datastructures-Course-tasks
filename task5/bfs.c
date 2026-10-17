#include "queue.c"
#include "weightedGraph.c"
#include <math.h>

typedef struct {
  int dist;
  Node *parent;
} Parent;

Parent *newParent() {
  Parent *p = calloc(1, sizeof(Parent));
  p->dist = INFINITY;
  return p;
}

void initParent(Graph *G, Node *s) {
  for (int i = G->N; i--;) {
    G->node[i].d = newParent();
  }
  ((Parent *)s->d)->dist = 0;
}

void bfs(Graph *G, Node *s) {
  initParent(G, s);
  Queue *q = newQueue(G->N - 1);
  addToQueue(q, s);
  while (!queueEmpty(q)) {
    Node *n = nextInQueue(q);
    for (WEdge *e = n->edge; e; e = e->next) {
      Parent *p = e->to->d;
      if (p->dist == INFINITY) {
        p->dist = ((Parent *)n->d)->dist + 1;
        p->parent = n;
        addToQueue(q, e->to);
      }
    }
  }
}
