#ifndef BINTREE_H
#define BINTREE_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct BinNode BinNode;
typedef struct BinNodeTree BinNodeTree;

// Individual node in a binary tree.
struct BinNode {
  const void *value;
  const void *value2;
  BinNodeTree *motherTree;
  BinNode *parent;
  BinNode *left;
  BinNode *right;
};

// The binary tree, which can be assigned a sorting function depending on
// datatype sorted.
struct BinNodeTree {
  BinNode *root;
  int (*sort)(const void *, const void *);
};

// Adds a node to the tree.
void addNode(BinNode *parent, BinNode *child);

// Adds a value to the tree.
bool appendValue(BinNodeTree *tree, const void *value);

// Calculates the depth of the tree by recursively calculating the depth of each
// branch.
int findDepth(BinNode *root);

// A method for putting the nodes from the binary tree into an array tree sorted
// by level.
bool toArray(const BinNodeTree *tree, BinNode ***out, size_t *outLength);

void *popSmallest(const BinNodeTree *tree);

#endif // BINTREE_H
