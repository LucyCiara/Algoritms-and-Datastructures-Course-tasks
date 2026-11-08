#include "map.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void bfmInit(ByteFreqMap *m) {
  for (size_t i = 0; i < BYTE_MAP_SIZE; i++)
    m->counts[i] = 0;
}

void bfmAdd(ByteFreqMap *m, unsigned char key) { m->counts[key]++; }

size_t bfmGet(const ByteFreqMap *m, unsigned char key) {
  return m->counts[key];
}

void bfmCountBuffer(ByteFreqMap *m, const unsigned char *buf, size_t len) {
  for (size_t i = 0; i < len; i++)
    m->counts[buf[i]]++;
}

int bfCompare(const void *a, const void *b) {
  const ByteFreq *x = a, *y = b;
  if (x->count < y->count)
    return -1;
  if (x->count > y->count)
    return 1;
  if (x->key < y->key)
    return -1;
  if ((x->key > y->key))
    return 1;
  return 0;
}

size_t bfmSorted(ByteFreqMap *m, ByteFreq out[BYTE_MAP_SIZE]) {
  size_t n = 0;
  for (size_t i = 0; i < BYTE_MAP_SIZE; i++) {
    if (m->counts[i] == 0)
      continue;
    out[n].key = (char)i;
    out[n].count = m->counts[i];
    n++;
  }
  qsort(out, n, sizeof(out[0]), &bfCompare);
  return n;
}
