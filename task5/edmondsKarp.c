#include "bfs.c"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int min(int a, int b) {
  if (a > b)
    return b;
  return a;
}

void printFlowPath(int flow, int *path, int n) {
  printf("%d  | ", flow);
  for (int i = n - 1; i >= 0; i--) {
    printf(" %d", path[i]);
  }
  printf("\n");
}

void edmondsKarp(Graph *G, Node *s, Node *t) {
  int flowTotal = 0;

  printf("flow | path\n");
  while (bfs(G, s, t)) {
    int flow = INT_MAX;
    int n = 1;
    int *path = calloc(n, sizeof(int));
    path[0] = t->num;
    for (Parent *p = (Parent *)t->d; p->parent != NULL;
         p = (Parent *)p->parent->d) {
      path = realloc(path, ++n * sizeof(int));
      path[n - 1] = p->parent->num;
      flow = min(flow, p->edge->capacity - p->edge->flow);
    }
    for (Parent *p = (Parent *)t->d; p->parent != NULL;
         p = (Parent *)p->parent->d) {
      p->edge->flow += flow;
      p->edge->opposite->flow -= flow;
    }
    printFlowPath(flow, path, n);
    flowTotal += flow;
  }
  printf("Total flow: %d\n", flowTotal);
}
