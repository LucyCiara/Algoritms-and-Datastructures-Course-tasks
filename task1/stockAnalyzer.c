
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

// Finds extremums by comparing adjacent data points on value change, and
// defines comparator depending on if its a maxima or minima. Returns true if
// it's an extremum and false if not.
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

// A method which takes an array of value changes, its length, and two pointers
// to integers, which can be set by the method. The method then returns an array
// containing an array of minima points and an array of maxima points {**minima,
// **maxima}, where each point is an int array {index, value-after-change}.
// O
int ***findExtremums(int *data, int length, int *sizeMinima, int *sizeMaxima) {
  int value = 0;

  *sizeMinima = 0;
  *sizeMaxima = 0;
  int ***extremums = (int ***)malloc(2 * sizeof(int **));
  int **minima = (int **)malloc(1 * sizeof(int *));
  int **maxima = (int **)malloc(1 * sizeof(int *));
  int comparator = 0;
  int i;
  for (i = 1; i < length; i++) {
    value += data[i - 1];
    bool isExtremum = compare(data[i], data[i - 1], &comparator);
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

  if (*sizeMinima == 0) {
    *sizeMinima += 1;
    minima = (int **)realloc(minima, *sizeMinima * sizeof(int *));
    minima[*sizeMinima - 1] = (int *)malloc(2 * sizeof(int));
    minima[*sizeMinima - 1][0] = 0;
    minima[*sizeMinima - 1][1] = 0 + data[0];
  } else if (*sizeMaxima == 0) {
    *sizeMaxima += 1;
    maxima = (int **)realloc(maxima, *sizeMaxima * sizeof(int *));
    maxima[*sizeMaxima - 1] = (int *)malloc(2 * sizeof(int));
    maxima[*sizeMaxima - 1][0] = 0;
    maxima[*sizeMaxima - 1][1] = 0 + data[0];
  }

  extremums[0] = minima;
  extremums[1] = maxima;
  return extremums;
}

// A method which takes an array of extremum points (arrays) {**minima,
// **maxima}, and the number of minima and maxima, and returns an ideal
// transaction (with the maximal gain) in the format of an array of points.
int **findIdealTransaction(int ***extremums, int minimaLength,
                           int maximaLength) {
  // Assigns memory to arrays, and set starting ideal transaction to the first
  // minima and maxima.
  int **idealTransaction = (int **)malloc(2 * sizeof(int *));
  idealTransaction[0] = extremums[0][0];
  idealTransaction[1] = extremums[1][0];

  // If the stocks only decrease, then the ideal transaction is to sell
  // immediately.
  if (minimaLength == 1 && maximaLength == 1 &&
      extremums[0][0][0] > extremums[1][0][0]) {
    idealTransaction[1] = extremums[0][0];
    return idealTransaction;
  }

  // Offsets the starting index of J, as to ensure that no minima is paired with
  // a maxima from before its index. It also saves a little time by reducing the
  // number of maxima to compare minima points to as the index of the minima
  // points increase.
  int startJ = 0;
  if (extremums[1][0][0] == 0) {
    startJ = 1;
  }

  for (int i = 0; i < minimaLength; i++) {
    for (int j = startJ; j < maximaLength; j++) {
      // Checks if the value gained between a minima and maxima is larger than
      // the current ideal transaction.
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

void free2dIntArray(int **array, int length) {
  for (int i = 0; i < length; i++) {
    free(array[i]);
  }
  free(array);
}

void free3dIntArray(int ***array, int length, int *lengths2) {
  for (int i = 0; i < length; i++) {
    free2dIntArray(array[i], lengths2[i]);
  }
  free(array);
}

void testStockAnalyzer(int repetitions, int n) {
  double time = 0;
  for (int i = 0; i < repetitions; i++) {
    int *data = malloc(n * sizeof(int));
    for (int j = 0; j < n; j++) {
      data[j] = rand() - RAND_MAX / 2;
    }

    clock_t t;
    t = clock();
    int minimaLength = 0;
    int maximaLength = 0;
    int ***extremums = findExtremums(data, n, &minimaLength, &maximaLength);
    int **idealTransaction =
        findIdealTransaction(extremums, minimaLength, maximaLength);
    t = clock() - t;
    time += ((double)t) / CLOCKS_PER_SEC;
    int lengths2[] = {minimaLength, maximaLength};
    free3dIntArray(extremums, 2, lengths2);
    free(idealTransaction);
    free(data);
  }
  time = time / repetitions;
  printf("Time for n=%d: %.5f\n", n, time);
}

int main() {
  // Runs once
  int lossGain[] = {-1, 3, -9, 2, 2, -1, 2, -1, -5};
  int length = sizeof(lossGain) / sizeof(int);
  int minimaLength = 0;
  int maximaLength = 0;

  // A loop running n times
  int ***extremums =
      findExtremums(lossGain, length, &minimaLength, &maximaLength);

  // A nested loop running ((n-k)/2)((n-k)/2+1)/2 times, or 1/2*((n-k)/2)^2 +
  // 1/2*((n-k)/2), where k ∈ {0, ..., n - 2}
  int **idealTransaction =
      findIdealTransaction(extremums, minimaLength, maximaLength);

  // Runs once
  int lengths2[] = {minimaLength, maximaLength};
  printf("The ideal transaction buys on day %d, and sells on day %d, with a "
         "profit of %d.\n",
         idealTransaction[0][0] + 1, idealTransaction[1][0] + 1,
         idealTransaction[1][1] - idealTransaction[0][1]);

  free3dIntArray(extremums, 2, lengths2);
  free(idealTransaction);

  // f(n) ≈ 1/2*((n-k)/2)^2 + 1/2*((n-k)/2) + n + 1, where k ∈ {0, .., n - 2}.
  // This approximation has been confirmed in a graphical plotting tool using
  // the output from testing the algorithm.
  //
  // f(n) ∈ O((n/2)^2) = O(n²), because
  // in the worst case scenario, every value in data would be an extremum (k =
  // 0).
  //
  // f(n) ∈ Ω(n), because in an ideal scenario, there would be only 2
  // extremums (k = n - 2).

  // Tests the algorithm for different n. The number of repetitions are higher
  // for lower n to make up for a lack in accuracy and to utilitze faster
  // completion time to run more simulations.
  srand(time(0));
  testStockAnalyzer(100, 1000);
  testStockAnalyzer(10, 10000);
  testStockAnalyzer(1, 100000);

  return 0;
}
