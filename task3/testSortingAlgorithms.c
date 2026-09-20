#include "quicksorts.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n = 200000;
char *formatString = "%24s | %6s | %8s | %25s\n";

int sum(int *array, int n) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += array[i];
  }
  return sum;
}

bool sumTest(int *sorted, int n, int expectedSum) {
  return sum(sorted, n) == expectedSum;
}

bool sortedTest(int *sorted, int n) {
  for (int i = 1; i < n; i++) {
    if (sorted[i] < sorted[i - 1])
      return false;
  }
  return true;
}

double timeTest(void (*sort)(int *, int, int), int *unSorted, int n) {
  clock_t t;
  t = clock();
  sort(unSorted, 0, n - 1);
  t = clock() - t;
  return ((double)t) / CLOCKS_PER_SEC;
}

void copyIntoArray(int *target, int *copy, int n) {
  for (int i = 0; i < n; i++)
    copy[i] = target[i];
}

void fillRandom(int *array, int n) {
  for (int i = 0; i < n; i++)
    array[i] = rand();
}

void fillRandomCapped(int *array, int n, int cap) {
  for (int i = 0; i < n; i++)
    array[i] = rand() % cap;
}

void printLine(int len) {
  for (int i = 0; i < len; i++) {
    printf("-");
  }
  printf("\n");
}

void reverseArray(int *array, int n) {
  for (int i = 0; i < n / 2; i++) {
    swap(&array[i], &array[n - i - 1]);
  }
}

void testSort(void (*sort)(int *, int, int), int *array, char *algorithmName) {

  int arrayCopy[n];
  copyIntoArray(array, arrayCopy, n);

  double sortTime = timeTest(&monoPivotQS, arrayCopy, n);
  bool sortSorted = sortedTest(arrayCopy, n);
  bool sortLossless = sumTest(arrayCopy, n, sum(array, n));
  char sortTimeStr[25];
  snprintf(sortTimeStr, sizeof(sortTimeStr), "%f", sortTime);
  printf(formatString, algorithmName, sortSorted ? "true" : "false",
         sortLossless ? "true" : "false", sortTimeStr);
}

int compare(const void *a, const void *b) {
  int *x = (int *)a;
  int *y = (int *)b;
  return (x > y) - (x < y);
}

int main() {
  srand(time(NULL));

  printf("\nRandom Array Test:\n");
  printf(formatString, "Sorting Algorithm", "Sorted", "Lossless", "Time");
  printLine(52);
  int testArray[n];
  fillRandom(testArray, n);
  testSort(&monoPivotQS, testArray, "Mono Pivot Quicksort");
  testSort(&dualPivotQS, testArray, "Dual Pivot Quicksort");
  testSort(&geeks4geeksSort, testArray, "G4G Dual Pivot Quicksort");

  printf("\nSorted Array Test:\n");
  printf(formatString, "Sorting Algorithm", "Sorted", "Lossless", "Time");
  printLine(52);
  qsort(testArray, n, sizeof(*testArray), &compare);
  testSort(&monoPivotQS, testArray, "Mono Pivot Quicksort");
  testSort(&dualPivotQS, testArray, "Dual Pivot Quicksort");
  testSort(&geeks4geeksSort, testArray, "G4G Dual Pivot Quicksort");

  printf("\nReversed Sorted Array Test:\n");
  printf(formatString, "Sorting Algorithm", "Sorted", "Lossless", "Time");
  printLine(52);
  reverseArray(testArray, n);
  testSort(&monoPivotQS, testArray, "Mono Pivot Quicksort");
  testSort(&dualPivotQS, testArray, "Dual Pivot Quicksort");
  testSort(&geeks4geeksSort, testArray, "G4G Dual Pivot Quicksort");

  printf("\nMany Duplicates Array Test:\n");
  printf(formatString, "Sorting Algorithm", "Sorted", "Lossless", "Time");
  printLine(52);
  fillRandomCapped(testArray, n, 100);
  testSort(&monoPivotQS, testArray, "Mono Pivot Quicksort");
  testSort(&dualPivotQS, testArray, "Dual Pivot Quicksort");
  testSort(&geeks4geeksSort, testArray, "G4G Dual Pivot Quicksort");
}
