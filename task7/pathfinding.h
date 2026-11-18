#ifndef PATHFINDING_H_
#define PATHFINDING_H_
#include "utils.h"

void dijkstra(Graph *G, Node *s, Node *t);
void dijkstraLogging(FILE *outF, Graph *G, Node *s, Node *t);

void getLandMarks(Graph *G, size_t *l1, size_t *l2, size_t *l3, size_t *l4);
void writeLandmarkMap(FILE *outF, Graph *G, size_t l);

#endif
