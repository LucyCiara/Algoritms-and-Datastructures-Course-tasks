#include "pathfinding.h"
#include "utils.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
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
#define POINTS_OF_INTEREST_FILE "interessepkt.txt"

Graph *createGraph() {
  // Create graph from file with edge info.
  FILE *edgeF = fopen(EDGE_FILE, "r");
  FILE *nodeF = fopen(NODE_FILE, "r");
  FILE *interestF = fopen(POINTS_OF_INTEREST_FILE, "r");
  Graph *G = newGraph(edgeF, nodeF, interestF);
  fclose(edgeF);
  fclose(nodeF);
  fclose(interestF);
  return G;
}

// The reversed graph is important in heuristic calculations, since the edges
// are one-directional.
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

// Finds the distance to and from every point in the graph and the landmarks.
// Writes this find to file.
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

// Extracts information about the landmarks  from files.
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
  // Test algorithm for every example dataset.
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
    printf("%-7i  %-7i  %-10i  %2i:%2i:%5.2f  %.2f\n", starts[i], ends[i],
           count, hours, minutes, seconds, timeInSeconds);

    // Assert info with answers and free ancestors.
    assert(hours == hourResult[i]);
    assert(minutes == minuteResult[i]);
    assert((int)seconds == secondResult[i]);
    freeAncestors(G);
  }
  freeLandmarkInfos(landmarkInfos, G, LN);
}

void testAlgorithmInput(void (*f)(Graph *, Node *, Node *, LandmarkInfo **),
                        char algorithmName[], int start, int end) {
  Graph *G = createGraph();

  // Get preprocessing data from file.
  size_t landmarks[LN];
  LandmarkInfo **landmarkInfos = readPreprocess(G, landmarks);

  printf("\n%s\n", algorithmName);
  printf("%-7s  %-7s  %-10s  %-11s  Computation Time\n", "Start", "End",
         "Node Count", "Trip Time");

  // Run algorithm.
  Node *s = &G->node[start], *t = &G->node[end];

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
  printf("%-7i  %-7i  %-10i  %2i:%2i:%5.2f  %.2f\n", start, end, count, hours,
         minutes, seconds, timeInSeconds);

  freeAncestors(G);
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

  printf("\n%s\n", algorithmName);

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

    // Write start and stop coordinates to a file.
    FILE *startStopF = fopen(outFStartAndStop, "w+");
    fprintf(startStopF, "%lf,%lf\n", s->latitude, s->longitude);
    fprintf(startStopF, "%lf,%lf\n", t->latitude, t->longitude);
    fclose(startStopF);
  }
  freeLandmarkInfos(landmarkInfos, G, LN);
}

void writeAlgorithmCoordInput(void (*f)(Graph *, Node *, Node *, char[],
                                        LandmarkInfo **),
                              char algorithmName[], int start, int end) {
  Graph *G = createGraph();

  // Get preprocessing data from file.
  size_t landmarks[LN];
  LandmarkInfo **landmarkInfos = readPreprocess(G, landmarks);

  printf("\n%s\n", algorithmName);

  char outFName[] = "coordinates.csv";
  char outFTrimmedName[] = "coordinatesTrimmed.csv";
  char outFStartAndStop[] = "startStop.csv";
  char outFPath[] = "path.csv";

  // Run coordinateWriting algorithm.
  Node *s = &G->node[start], *t = &G->node[end];
  f(G, s, t, outFName, landmarkInfos);

  FILE *pathF = fopen(outFPath, "w+");
  for (Ancestor *d = t->d; d->ancestor && d->ancestor->d != NULL;
       d = d->ancestor->d) {
    fprintf(pathF, "%f,%f\n", d->ancestor->latitude, d->ancestor->longitude);
  }
  fclose(pathF);

  freeAncestors(G);

  // Count number of lines in pathfinding file.
  FILE *inF = fopen(outFName, "r");
  size_t lines = countLines(inF);
  fclose(inF);
  printf("Number of nodes explored: %zu\n", lines);

  // Write start and stop coordinates to a file.
  FILE *startStopF = fopen(outFStartAndStop, "w+");
  fprintf(startStopF, "%lf,%lf\n", s->latitude, s->longitude);
  fprintf(startStopF, "%lf,%lf\n", t->latitude, t->longitude);
  fclose(startStopF);
  freeLandmarkInfos(landmarkInfos, G, LN);
}

////////////////////////////////////////////////////////////////////////////////
/// Main method
////////////////////////////////////////////////////////////////////////////////

#define POINTS_OF_INTEREST_FILE "interessepkt.txt"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Run program in the format of './main -flag'\n");
    return 1;
  }

  if (strcmp(argv[1], "-p") == 0 || strcmp(argv[1], "--preprocess") == 0) {
    preprocess();
  } else if (strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--test") == 0) {
    testAlgorithm(testDijkstraCorrect, "Dijkstra");
    testAlgorithm(testALTCorrect, "ALT");
    writeAlgorithmCoords(executeDijkstraWrite, "dijkstra");
    writeAlgorithmCoords(executeALTWrite, "ALT");
  } else if ((strcmp(argv[1], "-d") == 0 ||
              strcmp(argv[1], "--dijkstra") == 0 ||
              strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--alt") == 0 ||
              strcmp(argv[1], "-c") == 0 ||
              strcmp(argv[1], "--coordinates") == 0) &&
             argc >= 4) {
    size_t start = strtoul(argv[2], NULL, 10);
    size_t end = strtoul(argv[3], NULL, 10);
    if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--dijkstra") == 0) {
      if (argc == 4) {
        testAlgorithmInput(testDijkstraCorrect, "Dijkstra", start, end);
      } else if (argc == 5 &&
                 (strcmp(argv[4], "-c") || strcmp(argv[4], "--coordinates"))) {
        writeAlgorithmCoordInput(executeDijkstraWrite, "Dijkstra", start, end);
      }
    } else if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--alt") == 0) {
      if (argc == 4) {
        testAlgorithmInput(testALTCorrect, "ALT", start, end);
      } else if (argc == 5 && (strcmp(argv[4], "-c") == 0 ||
                               strcmp(argv[4], "--coordinates") == 0)) {
        writeAlgorithmCoordInput(executeALTWrite, "ALT", start, end);
      }
    }
  } else if ((strcmp(argv[1], "-i") == 0 ||
              strcmp(argv[1], "--points-of-interest") == 0) &&
             argc >= 4) {
    size_t start = strtoul(argv[2], NULL, 10);
    size_t CN = argc - 3;
    uint8_t codes[CN];
    for (size_t i = 3; i < argc; ++i) {
      codes[i - 3] = strtoul(argv[i], NULL, 10);
    }
    Graph *G = createGraph();
    size_t PN = 4;
    size_t *pointsOfInterest =
        dijkstraPointsOfInterest(G, &G->node[start], PN, CN, codes);
    printf("The closest points of interest are:\n");
    for (size_t i = 0; i < PN; ++i) {
      printf("%zu\n", pointsOfInterest[i]);
    }
  }
  return 0;
}
