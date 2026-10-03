#include "linkedNumber.c"
#include <stdio.h>
int main(int argc, char *argv[]) {
  char *numberStr = "527380";
  LinkedNumber number;
  createLinkedNumber(&number, numberStr);
  DigitNode digit = *number.tail;
  printf("\n%d", digit.digit);
  while (digit.previous != NULL) {
    digit = *digit.previous;
    printf("%d", digit.digit);
  }
  printf("\n");
}
