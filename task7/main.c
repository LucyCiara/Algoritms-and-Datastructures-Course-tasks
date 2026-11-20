#include "pathfinding.h"
#include "utils.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
/// Handling data
////////////////////////////////////////////////////////////////////////////////
#define EXAMPLES 12
#define NODE_FILE "noder.txt"
#define EDGE_FILE "kanter.txt"

Graph *createGraph() {
  // Create graph from file with edge info.
  FILE *edgeF = fopen(EDGE_FILE, "r");
  FILE *nodeF = fopen(NODE_FILE, "r");
  Graph *G = newGraph(edgeF, nodeF);
  fclose(edgeF);
  fclose(nodeF);
  return G;
}

Graph *createReverseGraph() {
  // Create graph from file with edge info.
  FILE *edgeF = fopen(EDGE_FILE, "r");
  FILE *nodeF = fopen(NODE_FILE, "r");
  Graph *R = newReverseGraph(edgeF, nodeF);
  fclose(edgeF);
  fclose(nodeF);
  return R;
}

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

////////////////////////////////////////////////////////////////////////////////
/// ALT pre-processing
////////////////////////////////////////////////////////////////////////////////

#define LANDMARK_FILENAMES (char *[4]){"l1.txt", "l2.txt", "l3.txt", "l4.txt"}
#define LN 4

void preprocess() {
  Graph *G = createGraph();
  Graph *R = createReverseGraph();

  size_t l1, l2, l3, l4;
  getLandMarks(G, &l1, &l2, &l3, &l4);
  size_t landmarks[] = {l1, l2, l3, l4};
  for (int i = 0; i < LN; ++i) {
    FILE *outF = fopen(LANDMARK_FILENAMES[i], "w+");
    writeLandmarkMap(outF, G, R, landmarks[i]);
    fclose(outF);
    freeAncestors(G);
  }

  free(G);
}

LandmarkInfo **readPreprocess(Graph *G, size_t landmarks[LN]) {
  LandmarkInfo **landmarkInfos = malloc(LN * sizeof *landmarkInfos);
  for (int i = 0; i < LN; ++i) {
    FILE *inF = fopen(LANDMARK_FILENAMES[i], "r");
    LandmarkInfo *landmarkInfo = readLandmarkMap(inF, G, &landmarks[i]);
    landmarkInfos[i] = landmarkInfo;
    fclose(inF);
  }
  return landmarkInfos;
}

////////////////////////////////////////////////////////////////////////////////
/// Testing algorithms' correctness using test data
////////////////////////////////////////////////////////////////////////////////

void testDijkstraCorrect(Graph *G, Node *s, Node *t,
                         LandmarkInfo **landmarkInfos) {
  dijkstra(G, s, t);
}

void testALTCorrect(Graph *G, Node *s, Node *t, LandmarkInfo **landmarkInfos) {
  ALT(G, s, t, LN, landmarkInfos);
}

void testAlgorithm(void (*f)(Graph *, Node *, Node *, LandmarkInfo **),
                   char algorithmName[]) {
  Graph *G = createGraph();

  // Define test data.
  int starts[EXAMPLES], ends[EXAMPLES], nodeCountResult[EXAMPLES],
      hourResult[EXAMPLES], minuteResult[EXAMPLES], secondResult[EXAMPLES];
  defineTestData(starts, ends, nodeCountResult, hourResult, minuteResult,
                 secondResult);

  // Get preprocessing data from file.
  size_t landmarks[LN];
  LandmarkInfo **landmarkInfos = readPreprocess(G, landmarks);

  printf("\n%s\n", algorithmName);
  printf("%-7s  %-7s  %-10s  %-11s  Computation Time\n", "Start", "End",
         "Node Count", "Trip Time");
  // Test ALT algorithm for every example dataset.
  for (int i = 0; i < EXAMPLES; ++i) {

    // Run algorithm.
    Node *s = &G->node[starts[i]], *t = &G->node[ends[i]];

    clock_t time;
    time = clock();
    f(G, s, t, landmarkInfos);
    time = clock() - time;
    double timeInSeconds = ((double)time) / CLOCKS_PER_SEC;

    // Get path infos and print them.
    long long dist = ((Ancestor *)t->d)->dist;
    double seconds = centiSecondsToSeconds(dist);
    int minutes = extractMinutes(&seconds);
    int hours = extractHours(&minutes);
    int count = 1;
    for (Ancestor *a = (Ancestor *)t->d; a->ancestor; a = a->ancestor->d)
      ++count;
    printf("%-7i  %-7i  %-10i  %-2i:%-2i:%-5.2f  %.2f\n", starts[i], ends[i],
           count, hours, minutes, seconds, timeInSeconds);

    // Assert info with answers and free ancestors.
    assert(hours == hourResult[i]);
    assert(minutes == minuteResult[i]);
    assert((int)seconds == secondResult[i]);
    freeAncestors(G);
  }
  freeLandmarkInfos(landmarkInfos, G, LN);
}

////////////////////////////////////////////////////////////////////////////////
/// Writing coordinates for the first test data entry for displaying on a map.
////////////////////////////////////////////////////////////////////////////////

#define COORD_ENTRY_LIMIT 1000
#define COORD_FILE_NAME "Coordinates"
#define COORD_FILE_TYPE ".csv"

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

void executeDijkstraWrite(Graph *G, Node *s, Node *t, char outFName[],
                          LandmarkInfo **landmarkInfos) {
  // Write pathfinding to file.
  FILE *outF = fopen(outFName, "w+");
  dijkstraLogging(outF, G, s, t);
  fclose(outF);
}

void executeALTWrite(Graph *G, Node *s, Node *t, char outFName[],
                     LandmarkInfo **landmarkInfos) {
  // Write pathfinding to file.
  FILE *outF = fopen(outFName, "w+");
  ALTLogging(outF, G, s, t, LN, landmarkInfos);
  fclose(outF);
}

void writeAlgorithmCoords(void (*f)(Graph *, Node *, Node *, char[],
                                    LandmarkInfo **),
                          char algorithmName[]) {
  Graph *G = createGraph();

  // Get preprocessing data from file.
  size_t landmarks[LN];
  LandmarkInfo **landmarkInfos = readPreprocess(G, landmarks);

  printf("%s\n", algorithmName);

  for (size_t i = 0; i < EXAMPLES; ++i) {
    size_t outFNameLen = strlen(algorithmName) + strlen(COORD_FILE_NAME) +
                         strlen(COORD_FILE_TYPE) + 1 + (i / 10) + 1;
    char outFName[outFNameLen];
    snprintf(outFName, outFNameLen, "%s%s%zu%s", algorithmName, COORD_FILE_NAME,
             i, COORD_FILE_TYPE);

    size_t outFTrimmedNameLen = outFNameLen + strlen("Trimmed");
    char outFTrimmedName[outFTrimmedNameLen];
    snprintf(outFTrimmedName, outFTrimmedNameLen, "%s%s%s%zu%s", algorithmName,
             "Trimmed", COORD_FILE_NAME, i, COORD_FILE_TYPE);

    size_t outFStartAndStopLen = strlen(algorithmName) + strlen("StartStop") +
                                 strlen(COORD_FILE_TYPE) + 1 + (i / 10) + 1;
    char outFStartAndStop[outFStartAndStopLen];
    snprintf(outFStartAndStop, outFStartAndStopLen, "%s%s%zu%s", algorithmName,
             "StartStop", i, COORD_FILE_TYPE);

    // Define test data
    int starts[EXAMPLES], ends[EXAMPLES], nodeCountResult[EXAMPLES],
        hourResult[EXAMPLES], minuteResult[EXAMPLES], secondResult[EXAMPLES];
    defineTestData(starts, ends, nodeCountResult, hourResult, minuteResult,
                   secondResult);

    // Run coordinateWriting algorithm.
    Node *s = &G->node[starts[i]], *t = &G->node[ends[i]];
    f(G, s, t, outFName, landmarkInfos);
    freeAncestors(G);

    // Count number of lines in pathfinding file.
    FILE *inF = fopen(outFName, "r");
    size_t lines = countLines(inF);
    fclose(inF);
    printf("Number of nodes explored: %zu\n", lines);

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
  freeLandmarkInfos(landmarkInfos, G, LN);
}

////////////////////////////////////////////////////////////////////////////////
/// Main method
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
  // // Opens a file for writing visitations of the algorithm.
  // FILE *coordOutF = fopen("visitation.txt", "w+");

  preprocess();
  testAlgorithm(testDijkstraCorrect, "Dijkstra");
  testAlgorithm(testALTCorrect, "ALT");
  // writeAlgorithmCoords(executeDijkstraWrite, "dijkstra");
  // writeAlgorithmCoords(executeALTWrite, "ALT");
  //
  return 0;
}
