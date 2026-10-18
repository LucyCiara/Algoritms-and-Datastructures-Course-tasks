#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// A weighted edge struct containing information about other outgoing edges from
// its node, its opposite edge, the node it connects to, its flow capacity and
// its current flow.
typedef struct wEdge {
  struct wEdge *next;
  struct wEdge *opposite;
  struct node *to;
  int capacity;
  int flow;
} WEdge;

// A node struct which keeps track of its outgoing edge (with recursive info
// about its next edge), its node number, and a generic data *d, which will be
// used to attach a Parent struct.
typedef struct node {
  WEdge *edge;
  int num;
  void *d;
} Node;

// A graph struct containing info about its number of nodes and edges, and an
// array of its nodes.
typedef struct {
  int N, E;
  Node *node;
} Graph;

// Finds the source and sink.
void findSinkSource(Graph *G, Node **s, Node **t) {
  // Creates arrays mimicing the graph's node array, where index 0 is node 0,
  // etc. This way we can track the number of edges going in and out of each
  // node.
  int *in = calloc(G->N, sizeof(int));
  int *out = calloc(G->N, sizeof(int));

  // Iterates through each node and each of its edges to count the number of
  // outgoing and incoming edges for each node (not counting the 0 capacity
  // edges)
  for (int i = 0; i < G->N; i++) {
    for (WEdge *e = G->node[i].edge; e; e = e->next) {
      if (e->capacity > 0) {
        ++out[i];
        ++in[e->to->num];
      }
    }
  }

  // Checks the incoming and outgoing arrays and sets s and t to the node with 0
  // incoming and 0 outgoing edges respectively.
  for (int i = 0; i < G->N; i++) {
    if (in[i] == 0)
      *s = &G->node[i];
    if (out[i] == 0)
      *t = &G->node[i];
  }
  free(in);
  free(out);
}

// Sets each edge's opposite to an existing opposite.
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

// Creates a graph from a file.
Graph *newGraph(FILE *f, Node **s, Node **t) {
  // Initiates a new graph and sets its number of nodes and edges to the first 2
  // numbers of the file.
  Graph *G = (Graph *)malloc(sizeof(Graph));
  fscanf(f, "%i %i\n", &G->N, &G->E);

  // Allocates an array of nodes in the graph, and scans information about the
  // nodes and edges from the file.
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

  // Sets the opposite edge of each edge.
  for (int i = 0; i < G->N; ++i) {
    Node *n = &G->node[i];
    for (WEdge *e = n->edge; e; e = e->next) {
      // Checks if the opposite exists (to prevent duplicates) and the return
      // from the connectReverse method (will proceed if the edge doesn't
      // have an existing opposite that gets set at its opposite).
      if (!e->opposite && !connectReverse(n, e)) {
        // Creates a capacity 0 edge as its opposite, and adds it as its last
        // next or only edge depending on if there are any edges.
        // The for loops are inefficient and could be changed as a way to
        // improve the performance of the program.
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

  findSinkSource(G, s, t);

  return G;
}
