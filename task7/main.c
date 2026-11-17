#include "pathfinding.h"
#include "utils.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
/// Handling test data
////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////
/// Testing Dijkstra's correctness using test data
////////////////////////////////////////////////////////////////////////////////

void testDijkstraCorrect(Graph *G) {
  int starts[EXAMPLES], ends[EXAMPLES], nodeCountResult[EXAMPLES],
      hourResult[EXAMPLES], minuteResult[EXAMPLES], secondResult[EXAMPLES];
  defineTestData(starts, ends, nodeCountResult, hourResult, minuteResult,
                 secondResult);

  for (int i = 0; i < EXAMPLES - 1; ++i) {
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

////////////////////////////////////////////////////////////////////////////////
/// Writing coordinates for the first test data entry for displaying on a map.
////////////////////////////////////////////////////////////////////////////////

#define COORD_ENTRY_LIMIT 1000
#define COORD_FILE_NAME "coordinates"

size_t countLines(FILE *f) {
  size_t i;
  char c;
  for (i = 0, c = getc(f); c != EOF; c = getc(f)) {
    if (c == '\n')
      ++i;
  }
  return i;
}

void trimFile(FILE *inF, FILE *outF, size_t lineCount) {
  size_t retainCounter = lineCount / COORD_ENTRY_LIMIT;
  char buffer[19];
  for (int i = 0; i < lineCount; ++i) {
    fscanf(inF, "%18[^\n]\n", buffer);
    if (i % (retainCounter + 1) == 0) {
      fprintf(outF, "%s\n", buffer);
    }
  }
}

void dijkstraWriteCoords(Graph *G) {
  char outFName[] = "coordinates.csv";
  char outFTrimmedName[] = "coordinatesTrimmed.csv";
  char outFStartAndStop[] = "startStop.csv";

  // Define test data
  int starts[EXAMPLES], ends[EXAMPLES], nodeCountResult[EXAMPLES],
      hourResult[EXAMPLES], minuteResult[EXAMPLES], secondResult[EXAMPLES];
  defineTestData(starts, ends, nodeCountResult, hourResult, minuteResult,
                 secondResult);

  // Write pathfinding to file.
  FILE *outF = fopen(outFName, "w+");
  Node *s = &G->node[starts[0]], *t = &G->node[ends[0]];
  dijkstraLogging(outF, G, s, t);
  fclose(outF);

  // Count number of lines in pathfinding file.
  FILE *inF = fopen(outFName, "r");
  size_t lines = countLines(inF);
  fclose(inF);

  // Trim pathfinding file.
  inF = fopen(outFName, "r");
  FILE *trimmedF = fopen(outFTrimmedName, "w+");
  trimFile(inF, trimmedF, lines);
  fclose(trimmedF);
  fclose(inF);

  // Write start and stop coordinates to a file.
  FILE *startStopF = fopen(outFStartAndStop, "w+");
  fprintf(startStopF, "%lf,%lf\n", s->latitude, s->longitude);
  fprintf(startStopF, "%lf,%lf\n", t->latitude, t->longitude);
  fclose(startStopF);
}

////////////////////////////////////////////////////////////////////////////////
/// Main method
////////////////////////////////////////////////////////////////////////////////

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
  // testDijkstraCorrect(G);
  dijkstraWriteCoords(G);

  return 0;
}
