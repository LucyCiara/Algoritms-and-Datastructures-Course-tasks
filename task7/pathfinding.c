#include "utils.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
/// Dijkstra Algorithms
////////////////////////////////////////////////////////////////////////////////

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
      shortenLogging(outF, q, n, e);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
/// ALT Algorithm
////////////////////////////////////////////////////////////////////////////////

void getLandMarks(Graph *G, size_t *l1, size_t *l2, size_t *l3, size_t *l4) {
  *l1 = 0, *l2 = 0, *l3 = 0, *l4 = 0;
  for (size_t i = 0; i < G->N; ++i) {
    if (G->node[i].latitude > G->node[*l1].latitude) {
      *l1 = i;
    } else if (G->node[i].longitude > G->node[*l2].longitude) {
      *l2 = i;
    } else if (G->node[i].latitude < G->node[*l3].latitude) {
      *l3 = i;
    } else if (G->node[i].longitude < G->node[*l4].longitude) {
      *l4 = i;
    }
  }
}

void writeLandmarkMap(FILE *outF, Graph *G, size_t l) {
  dijkstra(G, &G->node[l], NULL);
  fprintf(outF, "  %zu\n", l);
  for (size_t i = 0; i < G->N; ++i) {
    long long dist = ((Ancestor *)G->node[i].d)->dist;
    fprintf(outF, "%zu %lli\n", i, dist);
  }
}
