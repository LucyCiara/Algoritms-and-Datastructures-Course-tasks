
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
  if (a > 0 && b < 0) {
    *comparator = 1;
    return true;
  } else if (a < 0 && b > 0) {
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
  int value = 0;

  *sizeMaxima = 0;
  *sizeMaxima = 0;
  int ***extremums = (int ***)malloc(2 * sizeof(int **));
  int **minima = (int **)malloc(1 * sizeof(int *));
  int **maxima = (int **)malloc(1 * sizeof(int *));
  int comparator = 0;
  int i;
  for (i = 1; i < length; i++) {
    value += data[i - 1];
    // printf("a:[%d,%d]\n", i, data[i]);
    // printf("b:[%d,%d]\n", i - 1, data[i - 1]);
    bool isExtremum = compare(data[i], data[i - 1], &comparator);
    // printf("%d\n", isExtremum);
    if (isExtremum) {
      if (comparator > 0) {
        *sizeMinima += 1;
        minima = (int **)realloc(minima, *sizeMinima * sizeof(int *));
        minima[*sizeMinima - 1] = (int *)malloc(2 * sizeof(int));
        minima[*sizeMinima - 1][0] = i - 1;
        minima[*sizeMinima - 1][1] = value;

      } else {
        *sizeMaxima += 1;
        maxima = (int **)realloc(maxima, *sizeMaxima * sizeof(int *));
        maxima[*sizeMaxima - 1] = (int *)malloc(2 * sizeof(int));
        maxima[*sizeMaxima - 1][0] = i - 1;
        maxima[*sizeMaxima - 1][1] = value;
      }
    }
  }
  value += data[length - 1];
  if (comparator < 0) {
    *sizeMinima += 1;
    minima = (int **)realloc(minima, *sizeMinima * sizeof(int *));
    minima[*sizeMinima - 1] = (int *)malloc(2 * sizeof(int));
    minima[*sizeMinima - 1][0] = i - 1;
    minima[*sizeMinima - 1][1] = value;

  } else {
    *sizeMaxima += 1;
    maxima = (int **)realloc(maxima, *sizeMaxima * sizeof(int *));
    maxima[*sizeMaxima - 1] = (int *)malloc(2 * sizeof(int));
    maxima[*sizeMaxima - 1][0] = i - 1;
    maxima[*sizeMaxima - 1][1] = value;
  }
  extremums[0] = minima;
  extremums[1] = maxima;
  return extremums;
}

int **findIdealTransaction(int ***extremums, int minimaLength,
                           int maximaLength) {
  int **idealTransaction = (int **)malloc(2 * sizeof(int *));
  idealTransaction[0] = (int *)malloc(2 * sizeof(int));
  idealTransaction[0] = extremums[0][0];
  idealTransaction[1] = (int *)malloc(2 * sizeof(int));
  idealTransaction[1] = extremums[1][0];
  int startJ = 0;
  if (extremums[1][0][0] == 0) {
    startJ = 1;
  }
  for (int i = 0; i < minimaLength; i++) {
    for (int j = startJ; j < maximaLength; j++) {
      // printf("i: %d, j: %d\n", i, j);
      // printIntArray(extremums[0][i], 2);
      // printIntArray(extremums[1][j], 2);
      // printf("\n");
      if (extremums[1][j][1] - extremums[0][i][1] >
          idealTransaction[1][1] - idealTransaction[0][1]) {
        idealTransaction[0] = extremums[0][i];
        idealTransaction[1] = extremums[1][j];
      }
    }
    startJ += 1;
  }
  return idealTransaction;
}
int main() {
  int lossGain[] = {-1, 3, -9, 2, 2, -1, 2, -1, -5};
  int length = sizeof(lossGain) / sizeof(int);
  printIntArray(lossGain, length);
  int minimaLength = 0;
  int maximaLength = 0;
  int ***extremums =
      findExtremums(lossGain, length, &minimaLength, &maximaLength);

  printIntArrArray(extremums[0], minimaLength, 2);
  printIntArrArray(extremums[1], maximaLength, 2);

  int **idealTransaction =
      findIdealTransaction(extremums, minimaLength, maximaLength);
  printIntArrArray(idealTransaction, 2, 2);

  return 0;
}
