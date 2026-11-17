#ifndef UTILS_H_
#define UTILS_H_
///////////////////////////////////////////////////////////////////////////////
/// Imports
///////////////////////////////////////////////////////////////////////////////
#include <stddef.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
/// Weighted Graph Section
///////////////////////////////////////////////////////////////////////////////

// A weighted edge struct containing information about other outgoing edges from
// its node, its opposite edge, the node it connects to, its flow capacity and
// its current flow.
typedef struct wEdge {
  struct wEdge *next;
  struct wEdge *opposite;
  struct node *to;
  long long weight;
  long long heuristic;
} WEdge;

// A node struct which keeps track of its outgoing edge (with recursive info
// about its next edge), its node number, and a generic data *d, which will be
// used to attach a Parent struct.
typedef struct node {
  WEdge *edge;
  int num;
  void *d;
  struct fibNode *queueNode;
  double latitude, longitude;
} Node;

// A graph struct containing info about its number of nodes and edges, and an
// array of its nodes.
typedef struct {
  size_t N, E;
  Node *node;
} Graph;

// Ancestor struct, which allows keeping track of the shortest path to
// a node.
typedef struct {
  long long dist;
  Node *ancestor;
} Ancestor;

Graph *newGraph(FILE *edgeF, FILE *nodeF);

void initAncestor(Graph *G, Node *s);

////////////////////////////////////////////////////////////////////////////////
/// Fibonacci Heap Section
////////////////////////////////////////////////////////////////////////////////

// A node in a fibonacci heap.
typedef struct fibNode {
  size_t key, degree;
  struct fibNode *parent, *child, *left, *right;
  int mark;
  Node *vertex;
} FibNode;

// A fibonacci heap.
typedef struct {
  size_t n;
  FibNode *min;
} FibHeap;

FibHeap *createHeap();

FibNode *addToQueue(FibHeap *H, Node *vertex);

Node *popMin(FibHeap *H);

void decreaseKey(FibHeap *H, FibNode *x, int newKey);

void deleteNode(FibHeap *H, FibNode *x);

void emptyQueue(FibHeap *H);

////////////////////////////////////////////////////////////////////////////////
/// Both
////////////////////////////////////////////////////////////////////////////////

void shorten(FibHeap *H, Node *n, WEdge *e);

#endif
