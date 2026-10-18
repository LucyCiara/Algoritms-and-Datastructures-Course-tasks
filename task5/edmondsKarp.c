#include "bfs.c"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int min(int a, int b) {
  if (a > b)
    return b;
  return a;
}

// Prints the flow through a found path.
void printFlowPath(int flow, int *path, int n) {
  printf("%d  | ", flow);
  for (int i = n - 1; i >= 0; i--) {
    printf(" %d", path[i]);
  }
  printf("\n");
}

// Uses the edmonds-karp algorithm to find the maximum flow from s to t.
void edmondsKarp(Graph *G, Node *s, Node *t) {
  int flowTotal = 0;

  printf("flow | path\n");
  // Repeats the process whie the breadth-first-search is able to find paths.
  // A parent structure is made when this bfs is called.
  while (bfs(G, s, t)) {
    int flow = INT_MAX;
    // Creates a path array, and adds the sink as the first element.
    int n = 1;
    int *path = calloc(n, sizeof(int));
    path[0] = t->num;

    // Goes up the parent chain of the bfs-found solution, and adding them to
    // the path array and keeping track of the smallest capacity.
    for (Parent *p = (Parent *)t->d; p->parent != NULL;
         p = (Parent *)p->parent->d) {
      path = realloc(path, ++n * sizeof(int));
      path[n - 1] = p->parent->num;
      flow = min(flow, p->edge->capacity - p->edge->flow);
    }
    // Adds flow equal to the found minimum capacity to edges in the chain, and
    // subtracts from the edges going in the opposite direction.
    for (Parent *p = (Parent *)t->d; p->parent != NULL;
         p = (Parent *)p->parent->d) {
      p->edge->flow += flow;
      p->edge->opposite->flow -= flow;
    }
    printFlowPath(flow, path, n);
    // Adds upon the flowTotal.
    flowTotal += flow;
  }
  printf("Total flow: %d\n", flowTotal);
}
