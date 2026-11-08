#ifndef RUNLENGTH_H_
#define RUNLENGTH_H_
#include <stddef.h>

size_t encodeRL(char **output, char *inputBytes, size_t inputLength);

typedef struct {
  size_t unEncodedIndex;
  size_t unEncodedCount;
  char previousByte;
  size_t repetitionCount;
} UnencodingState;

void initUnencodingState(UnencodingState *state);

size_t decodeRL(char **output, char *inputBytes, size_t inputLength,
                UnencodingState *state);

#endif // !DEBUG
