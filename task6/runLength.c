#include "runLength.h"
#include "linkedList.h"
#include <iterator>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void addUnEncodedToOutput(char **output, char byte, size_t *length,
                          size_t *unEncodedIndex) {
  size_t u = *unEncodedIndex;
  if (u - (*output)[u] + 1 == (*length)++) {
    *output = realloc(*output, (*length) * sizeof(char));
    --(*output)[u];
  } else {
    *output = realloc(*output, (++*length) * sizeof(char));
    (*output)[*length - 2] = -1;
    *unEncodedIndex = *length - 2;
  }
  (*output)[*length - 1] = byte;
}

void addEncodedToOutput(char **output, char number, size_t *length) {
  *output = realloc(*output, (++*length) * sizeof(char));
  (*output)[*length - 1] = number;
}

size_t encodeRL(char **output, char *inputBytes, size_t inputLength) {
  size_t outputLen = 1;
  *output = (char *)calloc(outputLen, sizeof(char));

  size_t unEncodedIndex = 0;
  (*output)[unEncodedIndex] = 1;

  char currentByte;
  for (int i = 0; i < inputLength;) {
    currentByte = inputBytes[i];
    addUnEncodedToOutput(output, currentByte, &outputLen, &unEncodedIndex);
    int count = -1;
    while (++i < inputLength && inputBytes[i] == currentByte)
      ++count;
    if (count > 1)
      addEncodedToOutput(output, count, &outputLen);
  }
  return outputLen;
}
