#include "edmondsKarp.c"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("You must use the path of a file as an argument");
    printf("Example: ./main file1\n");
    return 1;
  }

  FILE *f = fopen(argv[1], "r");
  Node *source, *sink;
  Graph *G = newGraph(f, &source, &sink);
  edmondsKarp(G, source, sink);

  fclose(f);
  return 0;
}
