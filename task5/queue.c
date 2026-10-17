#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  void **tab;
  int start, end, num, max;
} Queue;

Queue *newQueue(int size) {
  Queue *q = (Queue *)malloc(sizeof(Queue));
  q->tab = (void *)malloc(size * sizeof(void *));
  q->max = size;
  q->start = q->end = q->num = 0;
  return q;
}

bool queueEmpty(Queue *q) { return !q->num; }

bool queueFull(Queue *q) { return q->num == q->max; }

void addToQueue(Queue *q, void *e) {
  if (queueFull(q))
    return;
  q->tab[q->end] = e;
  q->end = (q->end + 1) % q->max;
  ++q->num;
}

void *nextInQueue(Queue *q) {
  void *e;
  if (queueEmpty(q))
    return NULL;
  e = q->tab[q->start];
  q->start = (q->start + 1) % q->max;
  --q->num;
  return e;
}

void *peekQueue(Queue *q) {
  if (queueEmpty(q))
    return NULL;
  return q->tab[q->start];
}
