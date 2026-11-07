#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHUNK_READ_SIZE 4096

char *getFileLabel(char *filename) {
  char *period;
  if ((period = strchr(filename, '.')) != NULL) {
    char *fileLabel = (char *)calloc(period - filename, sizeof(char));
    strncpy(fileLabel, filename, period - filename);
    return fileLabel;
  } else
    return "file";
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

  FILE *f = fopen(filename, "rb");
  char buffer[CHUNK_READ_SIZE];
  size_t bytesRead;

  FILE *outputFile = fopen(strcat(getFileLabel(filename), ".lucybzip"), "wb");

  while ((bytesRead = fread(buffer, 1, CHUNK_READ_SIZE, f)) > 0) {
    for (int i = 0; i < bytesRead; i++)
      fwrite(&buffer[i], 1, 1, outputFile);
  }

  fclose(f);
  return 0;
}
