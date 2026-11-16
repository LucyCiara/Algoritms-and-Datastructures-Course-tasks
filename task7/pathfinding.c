#include "utils.h"

void dijkstra(Graph *G, Node *s, Node *t) {
  initAncestor(G, s);
  FibHeap *q = createHeap();
  addToQueue(q, s);
  while (q->n > 0) {
    Node *n = popMin(q);
    // if (n == t)
    //   return;
    for (WEdge *e = (WEdge *)n->edge; e; e = e->next)
      shorten(q, n, e);
  }
}
