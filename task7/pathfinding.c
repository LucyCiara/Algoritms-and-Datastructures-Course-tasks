#include "utils.h"
#include <stdio.h>

void dijkstra(Graph *G, Node *s, Node *t) {
  initAncestor(G, s);
  FibHeap *q = createHeap();
  addToQueue(q, s);
  while (q->n > 0) {
    Node *n = popMin(q);
    if (n == t) {
      emptyQueue(q);
      return;
    }
    for (WEdge *e = (WEdge *)n->edge; e; e = e->next)
      shorten(q, n, e);
  }
}

void dijkstraLogging(FILE *outF, Graph *G, Node *s, Node *t) {
  initAncestor(G, s);
  FibHeap *q = createHeap();
  addToQueue(q, s);
  while (q->n > 0) {
    Node *n = popMin(q);
    if (n == t) {
      emptyQueue(q);
      return;
    }
    for (WEdge *e = (WEdge *)n->edge; e; e = e->next) {
      fprintf(outF, "%i %lf %lf\n", n->num, n->latitude, n->longitude);
      shorten(q, n, e);
    }
  }
}
