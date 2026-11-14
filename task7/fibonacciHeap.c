// Based on:
// https://www.tutorialspoint.com/data_structures_algorithms/fibonacci_heap.htm
// and:
// https://www.programiz.com/dsa/fibonacci-heap
// Both were faulty or unfinished, so improvements had to be made.
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#define DEGREE 45

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

// INTERNAL: Creates a node.
FibNode *createNode(int key) {
  FibNode *n = (FibNode *)calloc(1, sizeof(FibNode));
  n->key = key;
  n->left = n->right = n;
  return n;
}

// Creates a heap.
FibHeap *createHeap() { return (FibHeap *)calloc(1, sizeof(FibHeap)); }

// Inserts a key into a heap, creating a node for it.
FibNode *insert(FibHeap *H, int key) {
  FibNode *n = createNode(key);
  if (!H->min)
    H->min = n;
  else {
    n->right = H->min->right;
    n->left = H->min;
    H->min->right->left = n;
    H->min->right = n;
    if (n->key < H->min->key)
      H->min = n;
  }
  ++(H->n);
  return n;
}

// INTERNAL: A method for making node x a child of node y.
void linkHeap(FibNode *x, FibNode *y) {
  x->left->right = x->right;
  x->right->left = x->left;

  if (y->child == NULL) {
    y->child = x;
    x->left = x;
  } else
    x->left = y->child->left;

  x->right = y->child;
  y->child->left->right = x;
  y->child->left = x;
  x->parent = y;
  x->mark = 0;

  ++(y->degree);
}

// INTERNAL: Finds new min and combines trees of the same degree.
void consolidate(FibHeap *H) {
  int d;
  // A heap of n = INT_MAX has a max degree tree of less than 45.
  FibNode *A[DEGREE], *y;
  for (int i = 0; i < DEGREE; ++i) {
    A[i] = NULL;
  }
  // Iterates through every sibling node of the min to combine nodes of equal
  // degree, and to find new mins where applicable.
  int siblings = 0;
  y = H->min;
  do {
    ++siblings;
    y = y->right;
  } while (y != H->min);
  for (int i = 0; i < siblings; ++i) {
    FibNode *next = y->right;
    d = y->degree;
    assert(d < DEGREE);
    // If another iterated node has had the same degree.
    while (A[d] != NULL) {
      // x is the previous node with the same degree.
      FibNode *x = A[d];
      // If y's key is larger, swaps x and y. y will always be the parent of x.
      if (y->key > x->key) {
        FibNode *n;
        n = y;
        y = x;
        x = n;
      }
      linkHeap(x, y);
      A[d] = NULL;
      ++d;
      assert(d < DEGREE);
    }
    A[d] = y;
    y = next;
  }

  // Finds the new H->min.
  y = A[d];
  H->min = y;
  FibNode *start = y;
  do {
    y = y->right;
    if (y->key < H->min->key) {
      H->min = y;
    }
  } while (y != start);
}

// Pops H's min, the smallest keyvalue.
FibNode *popMin(FibHeap *H) {
  FibNode *z = H->min;
  // Checks if z is non-NULL. If it's not, the NULL value will be returned
  // immediately after failing the condition.
  if (z) {
    FibNode *x = z->child;
    // If z has a child x, will bring it and its siblings up a step.-
    if (x) {
      FibNode *n = x;
      do {
        FibNode *next = n->right;
        n->right->left = n->left;
        n->left->right = n->right;
        n->left = H->min;
        n->right = H->min->right;
        H->min->right->left = n;
        H->min->right = n;
        n->parent = NULL;
        n = next;
      } while (n->parent != NULL);
    }
    // -Either way, removes z from heap and replaces it with its right sibling
    // if it exists, setting min to NULL otherwise.
    z->left->right = z->right;
    z->right->left = z->left;
    if (z == z->right) {
      H->min = NULL;
    } else {
      H->min = z->right;
      consolidate(H);
    }
    --(H->n);
  }
  return z;
}

// INTERNAL: Takes a node y and a direct subnode x of y, and moves it to the top
// as a sibling to min.
void cut(FibHeap *H, FibNode *x, FibNode *y) {
  if (x->right == x) {
    y->child = NULL;
  } else {
    x->right->left = x->left;
    x->left->right = x->right;
    if (y->child == x) {
      y->child = x->right;
    }
  }
  --(y->degree);

  x->left = H->min;
  x->right = H->min->right;
  H->min->right->left = x;
  H->min->right = x;
  x->parent = NULL;
  x->mark = 0;
}

// INTERNAL: Recursively pulls up a node and their unbroken chain of marked
// parents to the same level as the min node. Marked functions like a toggle,
// where only a parent that has had their child brought up once already will be
// pulled up themselves.
void cascadingCut(FibHeap *H, FibNode *y) {
  FibNode *z = y->parent;
  if (z) {
    if (!y->mark) {
      y->mark = 1;
    } else {
      cut(H, y, z);
      cascadingCut(H, z);
    }
  }
}

// Gives a new key value to a node if it's less than or equal to the existing
// key value, calling cut and cascadingCut to bring the new, lower value up.
void decreaseKey(FibHeap *H, FibNode *x, int newKey) {
  if (newKey > x->key) {
    return;
  }
  x->key = newKey;

  // If the parent node exists, and has a key value greater than x's key value,
  // brings x up to min's level.
  FibNode *y = x->parent;
  if (y && x->key < y->key) {
    cut(H, x, y);
    cascadingCut(H, y);
  }

  // If x's key is less than min's key, makes x the new min.
  if (x->key < H->min->key) {
    H->min = x;
  }
}

void deleteNode(FibHeap *H, FibNode *x) {
  assert(H->min->key >= 0);
  decreaseKey(H, x, -1);
  free(popMin(H));
}
