#include <math.h>
#include <stdio.h>
#include <time.h>
double method1(double x, double n) {
  //
  if (n == 1) {
    return x;
  } else {
    return x * method1(x, n - 1);
  }
}

double method2(double x, double n) {
  if (n == 1) {
    return x;
  } else if ((int)n & 1) {
    return x * method2(x * x, (n - 1) / 2);
  } else {
    return method2(x * x, n / 2);
  }
}

double nIncrementTest(double (*f)(double, double), double x, double n,
                      double reps, double *result, double *time) {
  *time = 0;
  clock_t t;
  for (int j = 0; j < reps; j++) {
    t = clock();
    f(x, n);
    t = clock() - t;
    *time += ((double)t) / CLOCKS_PER_SEC;
  }
  *time = *time / reps;
  *result = f(x, n);
  return n;
}

double repIncrementTest(double (*f)(double, double), double x, double n,
                        double reps, double *result, double *time) {
  *time = 0;
  clock_t t;
  for (int j = 0; j < reps; j++) {
    t = clock();
    f(x, n);
    t = clock() - t;
    *time += ((double)t) / CLOCKS_PER_SEC;
  }
  *result = f(x, n);
  return reps;
}

void printTest(double (*g)(double (*f)(double, double), double, double, double,
                           double *, double *),
               double (*f)(double, double), double x, double n, double reps,
               char *methodName) {
  char istr[10];

  double result;
  char rstr[7];

  double time;
  char tstr[9];

  double incrementor = g(f, x, n, reps, &result, &time);
  sprintf(istr, "%g", incrementor);
  sprintf(rstr, "%.4f", result);
  sprintf(tstr, "%.6f", time);
  printf("%10s %10s %20s %20s\n", istr, methodName, tstr, rstr);
}

int main() {
  int cap = 5000;
  int k = 5;
  double x = 1.002;

  printf("%10s %10s %20s %20s\n", "n", "method", "time", "result");

  for (int i = 1; i < k; i++) {
    double n = i * cap / k;
    double reps = 1000 * cap / n;

    for (int i = 0; i < 64; i++) {
      printf("-");
    }
    printf("\n");
    printTest(nIncrementTest, method1, x, n, reps, "method1");
    printTest(nIncrementTest, method2, x, n, reps, "method2");
    printTest(nIncrementTest, pow, x, n, reps, "pow");
  }

  for (int i = 0; i < 64; i++) {
    printf("-");
  }
  printf("\n\n\n\n\n\n\n");

  printf("%10s %10s %20s %20s\n", "reps", "method", "time", "result");

  for (int i = 0; i < 64; i++) {
    printf("-");
  }
  printf("\n");

  cap = 500000;
  double n = 5000;
  for (int i = 1; i < k; i++) {
    double reps = i * cap / k;

    for (int i = 0; i < 64; i++) {
      printf("-");
    }
    printf("\n");
    printTest(repIncrementTest, method2, x, n, reps, "method1");
    printTest(repIncrementTest, pow, x, n, reps, "pow");
  }
}
