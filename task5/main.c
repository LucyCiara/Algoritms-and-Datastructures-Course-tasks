#include "edmondsKarp.c"

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("You must use the path of a file as an argument, followed by source "
           "index and sink index.\n");
    printf("Example: ./main file1 0 7\n");
    return 1;
  }
  FILE *f = fopen(argv[1], "r");
  Graph *G = newGraph(f);
  edmondsKarp(G, &G->node[atoi(argv[2])], &G->node[atoi(argv[3])]);

  fclose(f);
  return 0;
}
