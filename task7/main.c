#include "pathfinding.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>

double centiSecondsToSeconds(long long cs) { return ((double)cs) / 100; }

int extractMinutes(double *seconds) {
  int minutes = *seconds / 60;
  *seconds = fmod(*seconds, 60);
  return minutes;
}

int main(int argc, char *argv[]) {
  FILE *f = fopen("kanter.txt", "r");
  Graph *G = newGraph(f);
  dijkstra(G, &G->node[2800567], &G->node[7705656]);
  long long dist = ((Ancestor *)G->node[7705656].d)->dist;
  double seconds = centiSecondsToSeconds(dist);
  int minutes = extractMinutes(&seconds);
  printf("\n%i:%.2f\n", minutes, seconds);
  return 0;
}
