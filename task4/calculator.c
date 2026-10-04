#include "linkedNumber.c"
#include <math.h>
#include <stdlib.h>
#include <time.h>

int randomIntInRange(int N) { return rand() % (N + 1); }

char *createRandomNumberString() {
  int length = randomIntInRange(9);
  char *numberString = (char *)malloc((length + 1) * sizeof(char));
  for (int i = 0; i < length; i++) {
    numberString[i] = randomIntInRange(9) + '0';
  }
  numberString[length] = '\0';
  return numberString;
}

bool testAdditon() {
  int reps = 1000;
  for (int i = 0; i < reps; i++) {
    char *number1 = createRandomNumberString();
    LinkedNumber linkedNumber1 = {0, NULL, NULL};
    createLinkedNumber(&linkedNumber1, number1);
    int number1Int = atoi(number1);

    char *number2 = createRandomNumberString();
    LinkedNumber linkedNumber2 = {0, NULL, NULL};
    createLinkedNumber(&linkedNumber2, number2);
    int number2Int = atoi(number2);

    LinkedNumber sumLinked = addLinkedNumbers(&linkedNumber1, &linkedNumber2);
    int sumInt = 0;
    DigitNode *digit = sumLinked.head;
    for (long long mult = 1; digit != NULL; mult *= 10) {
      sumInt += mult * digit->digit;
      digit = digit->next;
    }

    deepFreeLinkedNumber(&linkedNumber1);
    deepFreeLinkedNumber(&linkedNumber2);
    deepFreeLinkedNumber(&sumLinked);
    free(number1);
    free(number2);

    if (number1Int + number2Int != sumInt) {
      return false;
    }
  }
  return true;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  printf("Correct for 9 digits + 9 digits: %s\n",
         testAdditon() ? "true" : "false");
}
