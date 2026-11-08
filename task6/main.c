#include "runLength.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHUNK_READ_SIZE 4096

char *getFileLabel(char *filename, char *suffix) {
  char *period = strchr(filename, '.');
  size_t length = period ? (size_t)(period - filename) : strlen(filename);

  size_t totalLength = length + strlen(suffix) + 1;
  char *fileLabel = calloc(totalLength, sizeof(char));

  snprintf(fileLabel, totalLength, "%.*s%s", (int)length, filename, suffix);

  return fileLabel;
}

void encode(size_t *bytesRead, FILE *readFile, char *buffer, char *filename) {
  char *fileLabel = getFileLabel(filename, ".lucybzip");
  FILE *writeFile = fopen(fileLabel, "wb");
  free(fileLabel);
  while ((*bytesRead = fread(buffer, 1, CHUNK_READ_SIZE, readFile)) > 0) {
    char *output;
    size_t outputLen = encodeRL(&output, buffer, *bytesRead);
    for (size_t i = 0; i < outputLen; i++)
      fwrite(&output[i], 1, 1, writeFile);
    free(output);
  }
  fclose(writeFile);
}

void decode(size_t *bytesRead, FILE *readFile, char *buffer, char *filename) {
  char *fileLabel = getFileLabel(filename, "Out.txt");
  FILE *writeFile = fopen(fileLabel, "wb");
  free(fileLabel);
  UnencodingState state;
  initUnencodingState(&state);
  while ((*bytesRead = fread(buffer, 1, CHUNK_READ_SIZE, readFile)) > 0) {
    char *output;
    size_t outputLen = decodeRL(&output, buffer, *bytesRead, &state);
    for (size_t i = 0; i < outputLen; i++)
      fwrite(&output[i], 1, 1, writeFile);
    free(output);
  }
  fclose(writeFile);
}

int main(int argc, char *argv[]) {
  char *flag;
  char *filename;
  if (argc != 3) {
    printf("\nrun program like this: 'program -flag file'\n");
    return 1;
  }
  if (argv[1][0] == '-') {
    flag = argv[1];
    filename = argv[2];
  } else {
    flag = argv[2];
    filename = argv[1];
  }

  FILE *readFile = fopen(filename, "rb");
  char buffer[CHUNK_READ_SIZE];

  size_t bytesRead;
  if (strcmp(flag, "-e") == 0 || strcmp(flag, "--encode") == 0) {
    encode(&bytesRead, readFile, buffer, filename);
  } else if (strcmp(flag, "-d") == 0 || strcmp(flag, "--decode") == 0) {
    decode(&bytesRead, readFile, buffer, filename);
  } else {
    printf(
        "\nThe valid flags are: '-e' or '--encode' /// '-d' or '--decode'\n");
  }
  fclose(readFile);

  return 0;
}
