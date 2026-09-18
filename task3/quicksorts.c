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

void insertionSort(int *unSorted, int n) {
  for (int i = 1; i < n; ++i) {
    int key = unSorted[i];
    int j = i - 1;
    while (j >= 0 && unSorted[j] > key) {
      unSorted[j + 1] = unSorted[j];
    }
  }
}

void quartile4Sort(int *unSorted, int l, int h, int *q1, int *q3) {
  int m = (l + h) >> 1;
  *q1 = (l + m) >> 1;
  *q3 = (m + h) >> 1;

  // Optimal sorting of 4 numbers as discovered by Donald E. Knuth
  if (unSorted[l] > unSorted[*q3])
    swap(&unSorted[l], &unSorted[*q3]);
  if (unSorted[*q1] > unSorted[h])
    swap(&unSorted[*q1], &unSorted[h]);
  if (unSorted[l] > unSorted[*q1])
    swap(&unSorted[l], &unSorted[*q1]);
  if (unSorted[*q3] > unSorted[h])
    swap(&unSorted[q3], &unSorted[h]);
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
    if (unSorted[im] < q && im != ++il) {
      swap(&unSorted[im], &unSorted[il]);
    } else {
      while (unSorted[ih] > q)
        --ih;
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
  if (h - l > 3) {
    splitDual(unSorted, l, h, &lp, &rp);
    dualPivotQS(unSorted, l, lp);
    dualPivotQS(unSorted, lp, rp);
    dualPivotQS(unSorted, rp, h);
  } else
    quartile4Sort(unSorted, l, h, &lp, &rp);
}
