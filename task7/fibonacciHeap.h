#ifndef FIBONACCIHEAP_H_
#define FIBONACCIHEAP_H_
///////////////////////////////////////////////////////////////////////////////
/// Imports
///////////////////////////////////////////////////////////////////////////////
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
  int weight;
  int heuristic;
} WEdge;

// A node struct which keeps track of its outgoing edge (with recursive info
// about its next edge), its node number, and a generic data *d, which will be
// used to attach a Parent struct.
typedef struct node {
  WEdge *edge;
  int num;
  void *d;
  struct fibNode *queueNode;
} Node;

// A graph struct containing info about its number of nodes and edges, and an
// array of its nodes.
typedef struct {
  int N, E;
  Node *node;
} Graph;

Graph *newGraph(FILE *f);

void initAncestor(Graph *G, Node *s);

////////////////////////////////////////////////////////////////////////////////
/// Fibonacci Heap Section
////////////////////////////////////////////////////////////////////////////////

// A node in a fibonacci heap.
typedef struct fibNode {
  int key, degree;
  struct fibNode *parent, *child, *left, *right;
  int mark;
  Node *vertex;
} FibNode;

// A fibonacci heap.
typedef struct {
  int n;
  FibNode *min;
} FibHeap;

FibHeap *createHeap();

FibNode *addToQueue(FibHeap *H, Node *vertex);

Node *popMin(FibHeap *H);

void decreaseKey(FibHeap *H, FibNode *x, int newKey);

void deleteNode(FibHeap *H, FibNode *x);

////////////////////////////////////////////////////////////////////////////////
/// Both
////////////////////////////////////////////////////////////////////////////////

void shorten(FibHeap *H, Node *n, WEdge *e);

#endif
