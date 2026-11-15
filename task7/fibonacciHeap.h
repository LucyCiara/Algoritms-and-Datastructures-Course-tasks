#ifndef FIBONACCIHEAP_H_
#define FIBONACCIHEAP_H_

// A node in a fibonacci heap.
typedef struct fibNode {
  int key, degree;
  struct fibNode *parent, *child, *left, *right;
  int mark;
} FibNode;

// A fibonacci heap.
typedef struct {
  int n;
  FibNode *min;
} FibHeap;

FibHeap *createHeap();

FibNode *insert(FibHeap *H, int key);

FibNode *popMin(FibHeap *H);

void decreaseKey(FibHeap *H, FibNode *x, int newKey);

void deleteNode(FibHeap *H, FibNode *x);

#endif
