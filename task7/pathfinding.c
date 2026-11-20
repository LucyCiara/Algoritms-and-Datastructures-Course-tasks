#include "utils.h"
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
      free(q);
      return;
    }
    for (WEdge *e = (WEdge *)n->edge; e; e = e->next)
      shorten(q, n, e);
  }
  free(q);
}

void dijkstraLogging(FILE *outF, Graph *G, Node *s, Node *t) {
  initAncestor(G, s);
  FibHeap *q = createHeap();
  addToQueue(q, s);
  while (q->n > 0) {
    Node *n = popMin(q);
    fprintf(outF, "%lf,%lf\n", n->latitude, n->longitude);
    if (n == t) {
      emptyQueue(q);
      free(q);
      return;
    }
    for (WEdge *e = (WEdge *)n->edge; e; e = e->next) {
      shorten(q, n, e);
    }
  }
  free(q);
}

////////////////////////////////////////////////////////////////////////////////
/// ALT Algorithm
////////////////////////////////////////////////////////////////////////////////

void getLandMarks(Graph *G, size_t *l1, size_t *l2, size_t *l3, size_t *l4) {
  *l1 = 0, *l2 = 0, *l3 = 0, *l4 = 0;
  for (size_t i = 0; i < G->N; ++i) {
    if (G->node[i].latitude > G->node[*l1].latitude)
      *l1 = i;
    if (G->node[i].longitude > G->node[*l2].longitude)
      *l2 = i;
    if (G->node[i].latitude < G->node[*l3].latitude)
      *l3 = i;
    if (G->node[i].longitude < G->node[*l4].longitude)
      *l4 = i;
  }
}

void writeLandmarkMap(FILE *outF, Graph *G, Graph *R, size_t l) {
  dijkstra(G, &G->node[l], NULL);
  dijkstra(R, &R->node[l], NULL);
  fprintf(outF, "  %zu\n", l);
  for (size_t i = 0; i < G->N; ++i) {
    long long dist = ((Ancestor *)G->node[i].d)->dist;
    long long rDist = ((Ancestor *)R->node[i].d)->dist;
    if (dist == LLONG_MAX)
      dist = 0;
    if (rDist == LLONG_MAX)
      dist = 0;
    fprintf(outF, "%zu %lli %lli\n", i, dist, rDist);
  }
  freeAncestors(G);
}

LandmarkInfo *readLandmarkMap(FILE *inF, Graph *G, size_t *l) {
  LandmarkInfo *landmarkInfo = malloc(sizeof *landmarkInfo);
  landmarkInfo->from = calloc(G->N, sizeof(long long));
  landmarkInfo->to = calloc(G->N, sizeof(long long));
  fscanf(inF, "%zu", l);
  for (size_t i = 0; i < G->N; ++i) {
    long long dist = LLONG_MAX;
    long long rDist = LLONG_MAX;
    size_t index;
    fscanf(inF, "%zu %lli %lli", &index, &rDist, &dist);
    assert(i == index);
    landmarkInfo->from[i] = dist;
    landmarkInfo->to[i] = rDist;
  }
  return landmarkInfo;
}

long long max(long long x, long long y, long long z) {
  if (x >= y && x >= z)
    return x;
  if (y > x && y >= z)
    return y;

  return z;
}

void setHeruistic(Node *n, Node *t, int LN, LandmarkInfo **landmarkInfos) {
  long long heuristic = 0;
  for (int i = 0; i < LN; ++i) {
    long long candidate1 = 0;
    candidate1 = landmarkInfos[i]->to[t->num] - landmarkInfos[i]->to[n->num];
    long long candidate2 = 0;
    candidate2 =
        landmarkInfos[i]->from[n->num] - landmarkInfos[i]->from[t->num];
    heuristic = max(heuristic, candidate1, candidate2);
  }
  ((Ancestor *)n->d)->heuristic = heuristic;
}

void ALT(Graph *G, Node *s, Node *t, int LN, LandmarkInfo **landmarkInfos) {
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
      setHeruistic(e->to, t, LN, landmarkInfos);
      shorten(q, n, e);
    }
  }
  free(q);
}

void ALTLogging(FILE *outF, Graph *G, Node *s, Node *t, int LN,
                LandmarkInfo **landmarkInfos) {
  initAncestor(G, s);
  FibHeap *q = createHeap();
  addToQueue(q, s);
  while (q->n > 0) {
    Node *n = popMin(q);
    fprintf(outF, "%lf,%lf\n", n->latitude, n->longitude);
    if (n == t) {
      emptyQueue(q);
      return;
    }
    for (WEdge *e = (WEdge *)n->edge; e; e = e->next) {
      setHeruistic(e->to, t, LN, landmarkInfos);
      shorten(q, n, e);
    }
  }
  free(q);
}
