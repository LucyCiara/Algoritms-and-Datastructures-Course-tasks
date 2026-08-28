
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void printIntArray(int *input, int length) {
  printf("[");
  for (int i = 0; i < length - 1; i++) {
    printf("%d,", input[i]);
  }
  printf("%d]\n", input[length - 1]);
}

bool compare(int a, int b, int *comparator) {
  if ((a > b) && *comparator <= 0) {
    *comparator = 1;
    return true;
  } else if ((a < b) && *comparator >= 0) {
    *comparator = -1;
    return true;
  }
  return false;
}

int **findExtremums(int *data, int length, int *size) {
  if (length < 2) {
    printf("Input data needs to be longer than 2.");
    return 0;
  }
  *size = 0;
  int **extremums = malloc(1 * sizeof(int *));
  int comparator = 0;
  int i;
  for (i = 1; i < length; i++) {
    if (compare(data[i], data[i - 1], &comparator)) {
      *size += 1;
      extremums = (int **)realloc(extremums, *size * sizeof(int *));
      extremums[*size - 1] = (int *)malloc(2 * sizeof(int));
      extremums[*size - 1][0] = i - 1;
      extremums[*size - 1][1] = data[i - 1];
    }
  }
  *size += 1;
  extremums = (int **)realloc(extremums, *size * sizeof(int *));
  extremums[*size - 1] = (int *)malloc(2 * sizeof(int));
  extremums[*size - 1][0] = i - 1;
  extremums[*size - 1][1] = data[i - 1];

  return extremums;
}

int *findBiggest(int *data, int length) {
  int *max = malloc(2 * sizeof(int));
  max[0] = 0;
  max[1] = data[0];
  for (int i = 1; i < length; i++) {
    if (data[i] > max[1]) {
      max[0] = i;
      max[1] = data[i];
    }
  }
  return max;
}

int *findSmallest(int *data, int length) {
  int *min = malloc(2 * sizeof(int));
  min[0] = 0;
  min[1] = data[0];
  for (int i = 1; i < length; i++) {
    if (data[i] < min[1]) {
      min[0] = i;
      min[1] = data[i];
    }
  }
  return min;
}

int main() {
  int lossGain[] = {-1, 3, -9, 2, 2, -1, 2, -1, -5};
  int length = sizeof(lossGain) / sizeof(int);
  printIntArray(lossGain, length);

  int *max = findBiggest(lossGain, length);
  printIntArray(max, 2);
  free(max);
  int *min = findSmallest(lossGain, length);
  printIntArray(min, 2);
  free(min);

  int extremumsLength = 0;
  int **extremums = findExtremums(lossGain, length, &extremumsLength);
  printf("---\n");
  for (int i = 0; i < extremumsLength; i++) {
    printIntArray(extremums[i], 2);
  }
  free(extremums);

  return 0;
}
