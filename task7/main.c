#include "pathfinding.h"
#include "utils.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define EXAMPLES 12

double centiSecondsToSeconds(long long cs) { return ((double)cs) / 100; }

int extractMinutes(double *seconds) {
  int minutes = *seconds / 60;
  *seconds = fmod(*seconds, 60);
  return minutes;
}

int extractHours(int *minutes) {
  int hours = *minutes / 60;
  *minutes = *minutes % 60;
  return hours;
}

void defineTestData(int *starts, int *ends, int *nodeCountResult,
                    int *hourResult, int *minuteResult, int *secondResult) {
  // Defines the nodeIDs for the starts and ends of the paths.
  memcpy(starts,
         (int[]){2800567, 7705656, 647826, 136530, 7826348, 2948202, 339910,
                 1853145, 2503331, 2866570, 6441311, 3168086},
         EXAMPLES * sizeof(int));
  memcpy(ends,
         (int[]){7705656, 2800567, 136530, 647826, 2948202, 7826348, 1853145,
                 339910, 2866570, 2503331, 3168086, 6441311},
         EXAMPLES * sizeof(int));

  // Defines the correct number of nodes in the paths.
  memcpy(nodeCountResult,
         (int[]){334, 334, 408, 410, 1981, 2030, 3451, 3407, 7518, 7524, 5796,
                 5815},
         EXAMPLES * sizeof(int));

  // Defines the correct number of hours, minutes and seconds that it takes to
  // drive the path.
  memcpy(hourResult, (int[]){0, 0, 0, 0, 5, 5, 11, 11, 21, 21, 21, 21},
         EXAMPLES * sizeof(int));
  memcpy(minuteResult, (int[]){40, 40, 32, 33, 53, 53, 03, 03, 07, 06, 44, 45},
         EXAMPLES * sizeof(int));
  memcpy(secondResult, (int[]){47, 47, 44, 35, 13, 00, 16, 54, 07, 48, 14, 43},
         EXAMPLES * sizeof(int));
}

void testDijkstraCorrect(Graph *G) {
  int starts[EXAMPLES], ends[EXAMPLES], nodeCountResult[EXAMPLES],
      hourResult[EXAMPLES], minuteResult[EXAMPLES], secondResult[EXAMPLES];
  defineTestData(starts, ends, nodeCountResult, hourResult, minuteResult,
                 secondResult);

  for (int i = 0; i < EXAMPLES; ++i) {
    Node *s = &G->node[starts[i]], *t = &G->node[ends[i]];
    dijkstra(G, s, t);
    long long dist = ((Ancestor *)t->d)->dist;
    double seconds = centiSecondsToSeconds(dist);
    int minutes = extractMinutes(&seconds);
    int hours = extractHours(&minutes);
    int count = 1;
    for (Ancestor *a = (Ancestor *)t->d; a->ancestor; a = a->ancestor->d)
      ++count;
    printf("%i  %i  %i %lli  %i:%i:%.2f\n", starts[i], ends[i], count, dist,
           hours, minutes, seconds);
    assert(count == nodeCountResult[i]);
    assert(hours == hourResult[i]);
    assert(minutes == minuteResult[i]);
    assert((int)seconds == secondResult[i]);
  }
}

int main(int argc, char *argv[]) {
  // Create graph from file with edge info.
  FILE *edgeF = fopen("kanter.txt", "r");
  FILE *nodeF = fopen("noder.txt", "r");
  Graph *G = newGraph(edgeF, nodeF);
  fclose(edgeF);
  fclose(nodeF);

  // // Opens a file for writing visitations of the algorithm.
  // FILE *coordOutF = fopen("visitation.txt", "w+");

  // Tests dijkstra for
  testDijkstraCorrect(G);

  return 0;
}
