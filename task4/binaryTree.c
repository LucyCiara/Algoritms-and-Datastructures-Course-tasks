#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct BinNode BinNode;
typedef struct BinNodeTree BinNodeTree;

struct BinNode {
  const void *value;
  BinNodeTree *motherTree;
  BinNode *left;
  BinNode *right;
};

struct BinNodeTree {
  BinNode *root;
  int (*sort)(const void *, const void *);
};

void addNode(BinNode *parent, BinNode *child) {
  if (parent->motherTree->sort(parent->value, child->value) <= 0) {
    if (parent->left != NULL)
      addNode(child, parent->left);
    else
      parent->left = child;
  } else {
    if (parent->right != NULL)
      addNode(parent->right, child);
    else
      parent->right = child;
  }
}

bool appendValue(BinNodeTree *tree, const void *value) {
  BinNode *child = malloc(sizeof *child);
  if (!child)
    return false;
  child->value = value;
  child->motherTree = tree;
  child->left = NULL;
  child->right = NULL;
  if (tree->root != NULL)
    addNode(tree->root, child);
  else
    tree->root = child;
  return true;
}

int max(int a, int b) {
  if (a >= b)
    return a;
  return b;
}

int findDepth(BinNode *root) {
  if (!root)
    return 0;
  int leftDepth = findDepth(root->left);
  int rightDepth = findDepth(root->right);

  return max(leftDepth, rightDepth);
}
