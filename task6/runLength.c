#include "runLength.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_ENCODED 127
#define MIN_UNENCODED (-128)

void addUnEncodedToOutput(char **output, char byte, size_t *length,
                          size_t *unEncodedIndex) {
  size_t u = *unEncodedIndex;
  if (u - (*output)[u] + 1 == (*length)++ && (*output)[u] != MIN_UNENCODED) {
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
  size_t outputLen = 0;
  *output = (char *)calloc(1, sizeof(char));

  size_t unEncodedIndex = 0;
  (*output)[unEncodedIndex] = 0;

  char currentByte;
  for (size_t i = 0; i < inputLength;) {
    currentByte = inputBytes[i];
    addUnEncodedToOutput(output, currentByte, &outputLen, &unEncodedIndex);
    int count = 0;
    while (++i < inputLength && inputBytes[i] == currentByte &&
           ++count != MAX_ENCODED)
      ;
    if (count > 1)
      addEncodedToOutput(output, count - 1, &outputLen);
    else if (count == 1)
      addUnEncodedToOutput(output, currentByte, &outputLen, &unEncodedIndex);
  }
  return outputLen;
}

void initUnencodingState(UnencodingState *state) {
  state->previousByte = '\0';
  state->repetitionCount = 0;
  state->unEncodedIndex = 0;
  state->unEncodedCount = 0;
}

void addToDecodeOutput(char **output, char byte, size_t *length) {
  *output = realloc(*output, ++*length * sizeof(char));
  (*output)[*length - 1] = byte;
}

size_t decodeRL(char **output, char *inputBytes, size_t inputLength,
                UnencodingState *state) {
  size_t outputLen = 0;
  *output = (char *)calloc(1, sizeof(char));

  int unEncoded = 0;
  int repetitions = -1;

  if (state->unEncodedCount != 0) {
    unEncoded = state->unEncodedCount;
  } else if (state->repetitionCount != 0) {
    repetitions = state->repetitionCount;
  }

  for (size_t i = 0; i < inputLength;) {
    while (unEncoded > 0 && i < inputLength) {
      addToDecodeOutput(output, inputBytes[i++], &outputLen);
      --unEncoded;
    }
    if (i < inputLength) {
      if (inputBytes[i] < 0)
        unEncoded = -inputBytes[i];
      else
        repetitions = inputBytes[i];

      while (repetitions > -1) {
        addToDecodeOutput(output, inputBytes[i - 1], &outputLen);
        --repetitions;
      }
      ++i;
    }
  }
  state->unEncodedCount = unEncoded;
  state->repetitionCount = repetitions;
  return outputLen;
}
