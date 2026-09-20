#include <stdbool.h>

// void printArray(int *array, int n) {
//   printf("[");
//   for (int i = 0; i < n; i++) {
//     printf("%d,", array[i]);
//   }
//   printf("]\n");
// }

void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int median3Sort(int *unSorted, int l, int h) {
  // Forbedret fra eksempelet i boka
  int m = (l + h) >> 1;
  if (unSorted[l] > unSorted[m]) {
    swap(&unSorted[l], &unSorted[m]);
  }
  if (unSorted[m] > unSorted[h]) {
    swap(&unSorted[m], &unSorted[h]);
    if (unSorted[l] > unSorted[m]) {
      swap(&unSorted[l], &unSorted[m]);
    }
  }
  return m;
}

void quartile4Sort(int *unSorted, int l, int h, int *q1, int *q3) {
  if (h - l != 3) {
    int m = (l + h) >> 1;
    *q1 = (l + m) >> 1;
    *q3 = (m + h) >> 1;
  } else {
    *q1 = l + 1;
    *q3 = h - 1;
  }

  // Optimal sorting of 4 numbers as discovered by Donald E. Knuth
  if (unSorted[l] > unSorted[*q3])
    swap(&unSorted[l], &unSorted[*q3]);
  if (unSorted[*q1] > unSorted[h])
    swap(&unSorted[*q1], &unSorted[h]);
  if (unSorted[l] > unSorted[*q1])
    swap(&unSorted[l], &unSorted[*q1]);
  if (unSorted[*q3] > unSorted[h])
    swap(&unSorted[*q3], &unSorted[h]);
  if (unSorted[*q1] > unSorted[*q3])
    swap(&unSorted[*q1], &unSorted[*q3]);
}

int splitMono(int *unSorted, int l, int h) {
  int il, ih;
  int m = median3Sort(unSorted, l, h);
  int p = unSorted[m];
  swap(&unSorted[m], &unSorted[h - 1]);
  for (il = l, ih = h - 1;;) {
    while (unSorted[++il] < p)
      ;
    while (unSorted[--ih] > p)
      ;
    if (il >= ih)
      break;
    swap(&unSorted[il], &unSorted[ih]);
  }
  swap(&unSorted[il], &unSorted[h - 1]);
  return il;
}

// Based on algorithm from geeks4geeks, but written in a way more similar to the
// example from the book to make the differences clearer. Also added a few
// improvements.
void splitDual(int *unSorted, int l, int h, int *lp, int *rp) {
  int il, ih, im;
  int q1, q3;
  quartile4Sort(unSorted, l, h, &q1, &q3);
  int p = unSorted[q1], q = unSorted[q3];
  swap(&unSorted[q3], &unSorted[h - 1]);
  swap(&unSorted[q1], &unSorted[l + 1]);
  for (il = l + 1, ih = h - 2, im = il;;) {
    while (unSorted[++im] > p && unSorted[im] < q)
      ;
    if (unSorted[im] < q) {
      swap(&unSorted[im], &unSorted[++il]);
    } else {
      while (unSorted[--ih] > q)
        ;
      if (im >= ih)
        break;
      swap(&unSorted[ih], &unSorted[im]);
      if (unSorted[im] < p) {
        swap(&unSorted[im], &unSorted[++il]);
      }
    }
  }
  swap(&unSorted[il], &unSorted[l + 1]);
  swap(&unSorted[im], &unSorted[h - 1]);
  *lp = il, *rp = im;
}

// Geeks4Geeks minimally modified.
void partition(int *arr, int low, int high, int *lp, int *rp) {
  int q1, q3;
  quartile4Sort(arr, low, high, &q1, &q3);

  int p = arr[q1], q = arr[q3];
  // p is the left pivot, and q is the right pivot.
  int j = low + 1;
  int g = high - 1, k = low + 1;
  while (k <= g) {

    // if elements are less than the left pivot
    if (arr[k] < p) {
      swap(&arr[k], &arr[j]);
      j++;
    }

    // if elements are greater than or equal
    // to the right pivot
    else if (arr[k] >= q) {
      while (arr[g] > q && k < g)
        g--;
      swap(&arr[k], &arr[g]);
      g--;
      if (arr[k] < p) {
        swap(&arr[k], &arr[j]);
        j++;
      }
    }
    k++;
  }
  j--;
  g++;

  // bring pivots to their appropriate positions.
  swap(&arr[low], &arr[j]);
  swap(&arr[high], &arr[g]);

  // returning the indices of the pivots.
  *lp = j; // because we cannot return two elements
  *rp = g;
  // from a function.
}

void monoPivotQS(int *unSorted, int l, int h) {
  if (h - l > 2) {
    int p = splitMono(unSorted, l, h);
    monoPivotQS(unSorted, l, p);
    monoPivotQS(unSorted, p, h);
  } else
    median3Sort(unSorted, l, h);
}

void dualPivotQS(int *unSorted, int l, int h) {
  int lp, rp;
  if (h - l > 3 /* && splitDual(unSorted, l, h, &lp, &rp) */) {
    splitDual(unSorted, l, h, &lp, &rp);
    dualPivotQS(unSorted, l, lp);
    // if (lp != rp)
    dualPivotQS(unSorted, lp, rp);
    dualPivotQS(unSorted, rp, h);
  } else
    quartile4Sort(unSorted, l, h, &lp, &rp);
}

void geeks4geeksSort(int *arr, int low, int high) {
  if (low < high) {
    // lp means left pivot, and rp means right pivot.
    int lp, rp;
    partition(arr, low, high, &lp, &rp);
    geeks4geeksSort(arr, low, lp - 1);
    geeks4geeksSort(arr, lp + 1, rp - 1);
    geeks4geeksSort(arr, rp + 1, high);
  }
}
