#include "queue.c"
#include <stdbool.h>
#include <stdlib.h>

typedef struct BinNode BinNode;
typedef struct BinNodeTree BinNodeTree;

// Individual node in a binary tree.
struct BinNode {
  const void *value;
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
void addNode(BinNode *parent, BinNode *child) {
  // Sets the child's parent to parent. Will be overwritten if recursively
  // called.
  child->parent = parent;

  // Sorts smaller than and equal to the left, and larger than to the right.
  if (parent->motherTree->sort(parent->value, child->value) <= 0) {
    // Recursively calls on a child node if present, or inserts itself as child
    // node.
    if (parent->left != NULL)
      addNode(parent->left, child);
    else
      parent->left = child;
  } else {
    if (parent->right != NULL)
      addNode(parent->right, child);
    else
      parent->right = child;
  }
}

// Adds a value to the tree.
bool appendValue(BinNodeTree *tree, const void *value) {
  BinNode *child = malloc(sizeof *child);
  // Fails method if heap has no space.
  if (!child)
    return false;
  child->value = value;
  child->motherTree = tree;
  child->parent = NULL;
  child->left = NULL;
  child->right = NULL;

  // Starts a potentially recursive addNode chain if root exists, or places
  // child as root node.
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

int min(int a, int b) {
  if (a <= b)
    return a;
  return b;
}

// Calculates the depth of the tree by recursively calculating the depth of each
// branch.
int findDepth(BinNode *root) {
  if (!root)
    return -1;
  int leftDepth = findDepth(root->left);
  int rightDepth = findDepth(root->right);

  // Only the deepes branch counts.
  return max(leftDepth, rightDepth) + 1;
}

#define MAX_LEVELS 20 // This cut-off prevents overflows.

// A recursive method for filling an array with nodes by level.
static void fillLevelArray(BinNode *node, size_t index, BinNode **array,
                           size_t length) {
  if (!node || index >= length)
    return;
  array[index] = node;
  // recursively calls itself on its children.
  fillLevelArray(node->left, 2 * index + 1, array, length);
  fillLevelArray(node->right, 2 * index + 2, array, length);
}

// A method for putting the nodes from the binary tree into an array tree sorted
// by level.
bool toArray(const BinNodeTree *tree, BinNode ***out, size_t *outLength) {
  *out = NULL;
  *outLength = 0;
  if (!tree->root)
    return true;

  int depth = min(MAX_LEVELS - 1, findDepth(tree->root));

  // Calculates the length required of a maxed-out tree of the given depth. This
  // is because empty places are set to be NULL.
  size_t length = ((size_t)1 << (depth + 1)) - 1;
  BinNode **array = calloc(length, sizeof *array);

  // Fails method if heap is too small.
  if (!array)
    return false;

  fillLevelArray(tree->root, 0, array, length);
  *out = array;
  *outLength = length;
  return true;
}
