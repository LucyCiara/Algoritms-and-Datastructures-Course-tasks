#include "queue.c"
#include "weightedGraph.c"
#include <math.h>

// A parent struct which saves the distance in edges of a node, its parent node
// (node it was found from), and the edge it was found with.
typedef struct {
  int dist;
  Node *parent;
  WEdge *edge;
} Parent;

// A method for creating a new parent.
Parent *newParent() {
  Parent *p = calloc(1, sizeof(Parent));
  p->dist = INFINITY;
  return p;
}

// A method for initiating parents across a whole graph.
void initParent(Graph *G, Node *s) {
  for (int i = G->N; i--;) {
    G->node[i].d = newParent();
  }
  ((Parent *)s->d)->dist = 0;
}

// A breadth first search for finding a sink.
int bfs(Graph *G, Node *s, Node *t) {
  // Prepares the graph by defining and resetting parents across all nodes.
  initParent(G, s);
  // The size only needs to be N-1, because the sink is never added to queue
  Queue *q = newQueue(G->N - 1);
  addToQueue(q, s);

  // While there's still nodes in the queue, looks for the sink while updating
  // parent info.
  while (!queueEmpty(q)) {
    Node *n = nextInQueue(q);
    // Checks each outgoing edge from the node.
    for (WEdge *e = n->edge; e; e = e->next) {
      Parent *p = e->to->d;
      // Checks if the node is already visited or not, and if it still has
      // capacity left.
      if (p->dist == INFINITY && e->capacity - e->flow > 0) {
        p->edge = e;
        // Doubles as both keeping track of distance in edges from start and as
        // keeping track of its visit status.
        p->dist = ((Parent *)n->d)->dist + 1;
        p->parent = n;
        if (e->to == t)
          return 1;
        addToQueue(q, e->to);
      }
    }
  }
  return 0;
}
