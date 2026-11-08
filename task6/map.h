#ifndef MAP_H_
#define MAP_H_

#include <stddef.h>
#define BYTE_MAP_SIZE 256
typedef struct {
  size_t counts[BYTE_MAP_SIZE];
} ByteFreqMap;

typedef struct {
  char key;
  size_t count;
} ByteFreq;

void bfmInit(ByteFreqMap *m);

void bfmAdd(ByteFreqMap *m, unsigned char key);

size_t bfmGet(const ByteFreqMap *m, unsigned char key);

void bfmCountBuffer(ByteFreqMap *m, const unsigned char *buf, size_t len);

int bfCompare(const void *a, const void *b);

size_t bfmSorted(ByteFreqMap *m, ByteFreq out[BYTE_MAP_SIZE]);

#endif
