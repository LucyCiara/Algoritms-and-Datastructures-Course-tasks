
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

void printIntArrArray(int **input, int length1, int length2) {
  printf("[");
  for (int i = 0; i < length1; i++) {
    printf("[");
    for (int j = 0; j < length2 - 1; j++) {
      printf("%d,", input[i][j]);
    }
    printf("%d]", input[i][length2 - 1]);
  }
  printf("]\n");
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

int ***findExtremums(int *data, int length, int *sizeMinima, int *sizeMaxima) {
  if (length < 2) {
    printf("Input data needs to be longer than 2.");
    return 0;
  }
  *sizeMaxima = 0;
  *sizeMaxima = 0;
  int ***extremums = (int ***)malloc(2 * sizeof(int **));
  int **minima = (int **)malloc(1 * sizeof(int *));
  int **maxima = (int **)malloc(1 * sizeof(int *));
  int comparator = 0;
  int i;
  for (i = 1; i < length; i++) {
    if (compare(data[i], data[i - 1], &comparator)) {
      if (comparator < 1) {
        *sizeMinima += 1;
        minima = (int **)realloc(minima, *sizeMinima * sizeof(int *));
        minima[*sizeMinima - 1] = (int *)malloc(2 * sizeof(int));
        minima[*sizeMinima - 1][0] = i - 1;
        minima[*sizeMinima - 1][1] = data[i - 1];

      } else {
        *sizeMaxima += 1;
        maxima = (int **)realloc(maxima, *sizeMaxima * sizeof(int *));
        maxima[*sizeMaxima - 1] = (int *)malloc(2 * sizeof(int));
        maxima[*sizeMaxima - 1][0] = i - 1;
        maxima[*sizeMaxima - 1][1] = data[i - 1];
      }
    }
  }
  if (comparator > 1) {
    *sizeMinima += 1;
    minima = (int **)realloc(minima, *sizeMinima * sizeof(int *));
    minima[*sizeMinima - 1] = (int *)malloc(2 * sizeof(int));
    minima[*sizeMinima - 1][0] = i - 1;
    minima[*sizeMinima - 1][1] = data[i - 1];

  } else {
    *sizeMaxima += 1;
    maxima = (int **)realloc(maxima, *sizeMaxima * sizeof(int *));
    maxima[*sizeMaxima - 1] = (int *)malloc(2 * sizeof(int));
    maxima[*sizeMaxima - 1][0] = i - 1;
    maxima[*sizeMaxima - 1][1] = data[i - 1];
  }
  extremums[0] = minima;
  extremums[1] = maxima;
  return extremums;
}

int *findGlobalMaxima(int *data, int length) {
  int *max = (int *)malloc(2 * sizeof(int));
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

int *findGlobalMinima(int *data, int length) {
  int *min = (int *)malloc(2 * sizeof(int));
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

// int **findIdealTransaction(int *x400153Data, int length) {
//   int **idealTransaction = (int **)malloc() for ()
// }

int main() {
  int lossGain[] = {-1, 3, -9, 2, 2, -1, 2, -1, -5};
  int length = sizeof(lossGain) / sizeof(int);
  printIntArray(lossGain, length);

  int *max = findGlobalMaxima(lossGain, length);
  printIntArray(max, 2);
  free(max);
  int *min = findGlobalMinima(lossGain, length);
  printIntArray(min, 2);
  free(min);

  int minimaLength = 0;
  int maximaLength = 0;
  int ***extremums =
      findExtremums(lossGain, length, &minimaLength, &maximaLength);
  printIntArrArray(extremums[1], maximaLength, 2);
  free(extremums);

  return 0;
}
