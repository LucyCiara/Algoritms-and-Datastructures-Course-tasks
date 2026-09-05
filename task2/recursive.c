#include <math.h>
#include <stdio.h>
#include <time.h>

// The first method from the task text.
double method1(double x, double n) {
  if (n == 1) {
    return x;
  } else {
    // Calls itself incrementally to multiply x n times.
    return x * method1(x, n - 1);
  }
}

// The second method from the taskt text.
double method2(double x, double n) {
  if (n == 1) {
    return x;
    // This recursive method works by recursively passing x² as the x argument,
    // which will cause the exponent to be doubled each time. Clearly more
    // clever than method1. When n is odd, the method ensures it's passed in a
    // compatible way by breaking off an x.
  } else if ((int)n & 1) {
    return x * method2(x * x, (n - 1) / 2);
  } else {
    return method2(x * x, n / 2);
  }
}

// A test method where the exponent size n is incremented. The reps number is
// for averaging purposes with low n.
//
// Arg1 double (*f)(double, double): Takes a method, which it will perform time
// tests on.
//
// Arg2 double x: The base number to use in tests.
//
// Arg3 double n: The exponent to use in tests. Assumed to be incrementing.
//
// Arg4 double reps: The number of repetitions to test the method with.
//
// Arg5 double *result: The result of the method call. This way it can be
// observed after running this method, to see if the method's result differs
// from another's.
//
// Arg6 double *time: The resulting time average across reps test to perform the
// method with an exponent of n.
//
// return double: The exponent n, because nIncrementTests test an
// incrementing exponent.
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

// A test method where the number of reps is incremented, so the results
// shouldn't be divided by number of reps.
//
// Arg1 double (*f)(double, double): Takes a method, which it will perform time
// tests on.
//
// Arg2 double x: The base number to use in tests.
//
// Arg3 double n: The exponent to use in tests.
//
// Arg4 double reps: The number of repetitions to test the method with. Assumed
// to be incrementing.
//
// Arg5 double *result: The result of the method call. This way it can be
// observed after running this method, to see if the method's result differs
// from another's.
//
// Arg6 double *time: The resulting time taken for the method to be performed
// reps number of times.
//
// return double: The number of reps, because repIncrementTests test an
// incrementing number of repetitions.
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

// Method which tests a given method using a given test method and prints the
// results. The usage of function parameters is ugly, and I'll be looking for
// more readable alternatives in future practices.
//
// Arg1 double (*g)(double (*f)(double, double), double,
// double, double, double *, double *): The testing method.
//
// Arg2 double (*f)(double, double): The method to test.
//
// Arg3 double x: The base number.
//
// Arg4 double n: The exponent number. It's a double for compatibility with pow,
// but method1 and method2 need whole numbers.
//
// Arg5 double reps: The number of repetitions for the test to perform.
//
// Arg6 char *methodName: The name of the method, so the testing results can be
// properly attributed to it in the print.
//
// return int: 1 if terminated, 0 if successful.
int printTest(double (*g)(double (*f)(double, double), double, double, double,
                          double *, double *),
              double (*f)(double, double), double x, double n, double reps,
              char *methodName) {

  n = round(n); // Rounded just in case a whole number isn't passed, because
                // method1 and method2 only take whole numbers.

  // method1 and method2 only support numbers larger than 0, so this if
  // statement terminates the testing if n isn't.
  if (n <= 0) {
    return 1;
  }

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
  return 0;
}

int main() {
  int cap =
      5000;  // The exponent maximum (larger than this causes memory issues)
  int k = 5; // A constant for how many different n/rep tests are to be
             // performed under 5k
  double x = 1.002;

  // Prints out tests for method1, method2 and pow for incrrementing exponent n.
  printf("%10s %10s %20s %20s\n", "n", "method", "time", "result");
  for (int i = 1; i < k; i++) {
    double n = i * cap / k; // Precision loss is on purpose.
    double reps =
        1000 * cap / n; // The number of reps is 1000 times the cap/n, which
                        // causes higher n to get lower number of reps. This is
                        // because lower n require higher reps for consistency.

    for (int i = 0; i < 64; i++) {
      printf("-");
    }
    printf("\n");
    printTest(nIncrementTest, method1, x, n, reps, "method1");
    printTest(nIncrementTest, method2, x, n, reps, "method2");
    printTest(nIncrementTest, pow, x, n, reps, "pow");
  }
  printf("\n\n\n\n\n\n\n");

  // Prints out tests for method2 and pow, for incrementing number of
  // repetitions rep.
  printf("%10s %10s %20s %20s\n", "reps", "method", "time", "result");
  cap = 500000; // Here the cap is for the number of reps. It can be much larger
                // because it won't trigger memory issues.
  double n = 5000;
  for (int i = 1; i < k; i++) {
    double reps = i * cap / k; // Precjsion loss is on purpose.

    for (int i = 0; i < 64; i++) {
      printf("-");
    }
    printf("\n");
    printTest(repIncrementTest, method2, x, n, reps, "method2");
    printTest(repIncrementTest, pow, x, n, reps, "pow");
  }
}
