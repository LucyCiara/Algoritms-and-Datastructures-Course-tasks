#include "binaryTree.c"
#include <stdio.h>
#include <string.h>

// A method for printing out a tree
void printTree(BinNode **array, size_t length, int cellWidth,
               void (*fmt)(const void *value, char *buf, size_t size)) {
  size_t levels = 0;
  while (((size_t)1 << levels) - 1 < length)
    levels++;

  for (size_t r = 0; r < levels; r++) {
    int block = cellWidth << (levels - 1 - r); // width per slot in this row
    for (size_t i = ((size_t)1 << r) - 1; i < ((size_t)2 << r) - 1; i++) {
      char text[32] = "";
      if (array[i])
        fmt(array[i]->value, text, cellWidth);
      int len = (int)strlen(text);
      int pad = (block - len) / 2;
      printf("%*s%s%*s", pad, "", text, block - len - pad, "");
    }
    putchar('\n');
  }
}

// Reverses the strcmp to match task
int compareStrings(const void *a, const void *b) {
  return -1 * strcmp((const char *)a, (const char *)b);
}

void fmtString(const void *value, char *buf, size_t size) {
  snprintf(buf, size, "%s", (const char *)value);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Write words as arguments, and they'll be sorted in a binary tree. "
           "Example: './wordTree.c hello world skibidi rizz'\n");
    return 1;
  }

  BinNodeTree tree = {NULL, compareStrings};
  int cellWidth = 4;

  // Iterates for each argument and adds them to the tree.
  for (int i = 1; i < argc; i++) {
    if (!appendValue(&tree, argv[i])) {
      fprintf(stderr, "Out of memory\n");
      return 1;
    }
    int len = (int)strlen(argv[i]) + 1;
    if (len > cellWidth)
      cellWidth = len;
  }
  if (cellWidth > 31)
    cellWidth = 31;

  // Converts the binary tree into an array sorted by level.
  BinNode **array;
  size_t length;
  if (!toArray(&tree, &array, &length)) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  // Print the tree and free the array.
  printTree(array, length, cellWidth, fmtString);
  free(array);
  return 0;
}
