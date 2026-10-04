#include "linkedNumber.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int randomIntInRange(int N) { return rand() % (N + 1); }

// Creates a random string number of a max length of 9 digits, with an optional
// leading '-', for use in testing.
char *createRandomNumberString() {
  int length = randomIntInRange(9);
  bool negative = length > 0 && randomIntInRange(1) == 1;
  int start = negative ? 1 : 0;

  char *numberString = (char *)malloc((start + length + 1) * sizeof(char));
  if (negative)
    numberString[0] = '-';
  for (int i = 0; i < length; i++) {
    numberString[start + i] = randomIntInRange(9) + '0';
  }
  numberString[start + length] = '\0';
  return numberString;
}

// Converts a short LinkedNumber to a long long for use in testing.
long long linkedNumberToLongLong(LinkedNumber *number) {
  long long value = 0;
  long long mult = 1;
  for (DigitNode *digit = number->head; digit != NULL; digit = digit->next) {
    value += mult * digit->digit;
    mult *= 10;
  }
  return number->negative ? -value : value;
}

// Tests the accuracy of addition, subtraction and comparison on numbers of < 10
// digits (with random signs) because accuracy will carry on to longer numbers.
// It's capped to prevent integer overflow.
bool testOperations() {
  int reps = 1000;
  for (int i = 0; i < reps; i++) {
    char *number1 = createRandomNumberString();
    LinkedNumber linkedNumber1;
    createLinkedNumber(&linkedNumber1, number1);
    long long number1Value = atoll(number1);

    char *number2 = createRandomNumberString();
    LinkedNumber linkedNumber2;
    createLinkedNumber(&linkedNumber2, number2);
    long long number2Value = atoll(number2);

    LinkedNumber sumLinked = addLinkedNumbers(&linkedNumber1, &linkedNumber2);
    LinkedNumber diffLinked =
        subtractLinkedNumbers(&linkedNumber1, &linkedNumber2);
    int compared = compareLinkedNumbers(&linkedNumber1, &linkedNumber2);

    int expectedCompare =
        (number1Value > number2Value) - (number1Value < number2Value);
    bool correct =
        linkedNumberToLongLong(&sumLinked) == number1Value + number2Value &&
        linkedNumberToLongLong(&diffLinked) == number1Value - number2Value &&
        compared == expectedCompare &&
        // Zero must never be negative.
        !(sumLinked.length == 0 && sumLinked.negative) &&
        !(diffLinked.length == 0 && diffLinked.negative);

    if (!correct)
      printf("Failed on \"%s\" and \"%s\".\n", number1, number2);

    deepFreeLinkedNumber(&linkedNumber1);
    deepFreeLinkedNumber(&linkedNumber2);
    deepFreeLinkedNumber(&sumLinked);
    deepFreeLinkedNumber(&diffLinked);
    free(number1);
    free(number2);

    if (!correct)
      return false;
  }
  return true;
}

// The program can be run with numbers separated by the operators + and -,
// evaluated from left to right. Example: './calculator 12 + 30 - 100'
// The numbers can be negative as well: './calculator 5 - -3'
int main(int argc, char *argv[]) {
  srand(time(NULL));
  printf("Correct for signed 9 digits and 9 digits: %s\n",
         testOperations() ? "true" : "false");

  // Needs a number followed by at least one pair of an operator and a number,
  // so an even argc (the program name + an odd amount of arguments).
  if (argc < 4 || argc % 2 != 0) {
    printf("You need to write the numbers separated by + or - as arguments "
           "for the program. For example: './calculator 12345 + 67890' or "
           "'./calculator 12345 - 67890 + 987654321'. Numbers can be "
           "negative.\n");
    return 1;
  }

  LinkedNumber result;
  if (!createLinkedNumber(&result, argv[1])) {
    printf("%s is not a valid number.\n", argv[1]);
    return 1;
  }

  for (int i = 2; i < argc; i += 2) {
    bool isAdd = strcmp(argv[i], "+") == 0;
    bool isSubtract = strcmp(argv[i], "-") == 0;
    if (!isAdd && !isSubtract) {
      printf("%s is not a valid operator. Use + or -.\n", argv[i]);
      deepFreeLinkedNumber(&result);
      return 1;
    }

    LinkedNumber inputNum;
    if (!createLinkedNumber(&inputNum, argv[i + 1])) {
      printf("%s is not a valid number.\n", argv[i + 1]);
      deepFreeLinkedNumber(&result);
      return 1;
    }

    LinkedNumber next = isAdd ? addLinkedNumbers(&result, &inputNum)
                              : subtractLinkedNumbers(&result, &inputNum);
    // The old result and the input are no longer needed.
    deepFreeLinkedNumber(&result);
    deepFreeLinkedNumber(&inputNum);
    result = next;
  }

  printLinkedNumber(result);
  deepFreeLinkedNumber(&result);
  return 0;
}
