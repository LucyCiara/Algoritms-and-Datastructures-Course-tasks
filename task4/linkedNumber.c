#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// The node of a digit.
typedef struct DigitNodeStruct {
  int digit;
  struct DigitNodeStruct *previous;
  struct DigitNodeStruct *next;
} DigitNode;

// A whole number linked list. Digits are stored least significant first.
typedef struct {
  int length;
  bool negative; // Zero is never negative.
  struct DigitNodeStruct *head;
  struct DigitNodeStruct *tail;
} LinkedNumber;

// Internal method to add a new digit to the number.
DigitNode *newNode(int digit, DigitNode *n, DigitNode *p) {
  DigitNode *newDigit = (DigitNode *)(malloc(1 * sizeof(DigitNode)));
  newDigit->digit = digit;
  newDigit->next = n;
  newDigit->previous = p;
  return newDigit;
}

// Implements newNode to add a digit to to the front.
void putFirst(LinkedNumber *number, int digit) {
  // Adds new number with a given digit, and the old head as the next in the
  // link.
  DigitNode *new = newNode(digit, number->head, NULL);

  // Sets itself as the new head, and as the tail if there is none. Sets the old
  // head's previous as itself.
  number->head = new;
  if (!number->tail)
    number->tail = new;
  else
    new->next->previous = new;
  number->length++;
}

// Implements newNode to add a digit to the end.
void putLast(LinkedNumber *number, int digit) {
  // Adds new number with a given digit, and the old tail as the previous in the
  // link.
  DigitNode *new = newNode(digit, NULL, number->tail);

  // Sets itself as the new tail, and the head if there is none. Sets the old
  // tail's next as itself.
  number->tail = new;
  if (!number->head)
    number->head = new;
  else
    new->previous->next = new;
  number->length++;
}

// Sets a LinkedNumber to an empty (zero) number.
void initLinkedNumber(LinkedNumber *number) {
  number->length = 0;
  number->negative = false;
  number->head = NULL;
  number->tail = NULL;
}

// Removes leading zeros and prevents zero from being negative.
void trimLeadingZeros(LinkedNumber *number) {
  while (number->tail != NULL && number->tail->digit == 0) {
    DigitNode *last = number->tail;
    number->tail = last->previous;
    if (number->tail != NULL)
      number->tail->next = NULL;
    else
      number->head = NULL;
    free(last);
    number->length--;
  }
  if (number->length == 0)
    number->negative = false;
}

// Checks if the string only contains digits.
bool checkValidNumber(char *number) {
  for (int i = 0; number[i] != '\0'; i++) {
    if (!isdigit((unsigned char)number[i])) {
      return false;
    }
  }
  return true;
}

// Creates a LinkedNumber from a string of a whole number, with an optional
// leading '-'.
bool createLinkedNumber(LinkedNumber *emptyLinkedNumber, char *number) {
  initLinkedNumber(emptyLinkedNumber);

  int start = 0;
  bool negative = false;
  if (number[0] == '-') {
    negative = true;
    start = 1;
    // A lone "-" is not a number.
    if (number[1] == '\0')
      return false;
  }

  if (!checkValidNumber(number + start))
    return false;
  for (int i = start; number[i] != '\0'; i++) {
    // Uses character arithetic to convert from char to int.
    putFirst(emptyLinkedNumber, (number[i] - '0'));
  }

  trimLeadingZeros(emptyLinkedNumber);
  // "-0" and "-000" are just 0, which trimLeadingZeros already made positive.
  if (emptyLinkedNumber->length > 0)
    emptyLinkedNumber->negative = negative;
  return true;
}

int biggerOfTwo(int num1, int num2) {
  if (num1 > num2)
    return num1;
  else
    return num2;
}

int smallerOfTwo(int num1, int num2) {
  if (num2 < num1)
    return num2;
  else
    return num1;
}

// Compares the sizes of two numbers while ignoring their signs and leading
// zeros. Returns 1 if |num1| > |num2|, -1 if |num1| < |num2|, and 0 if equal.
int compareMagnitudes(LinkedNumber *num1, LinkedNumber *num2) {
  DigitNode *n1 = num1->tail;
  DigitNode *n2 = num2->tail;
  int length1 = num1->length;
  int length2 = num2->length;

  // Skips leading zeros, and shortens the effective length accordingly.
  while (n1 != NULL && n1->digit == 0) {
    n1 = n1->previous;
    length1--;
  }
  while (n2 != NULL && n2->digit == 0) {
    n2 = n2->previous;
    length2--;
  }

  if (length1 != length2)
    return length1 > length2 ? 1 : -1;

  // Same length, so the first differing digit from the top decides.
  while (n1 != NULL) {
    if (n1->digit != n2->digit)
      return n1->digit > n2->digit ? 1 : -1;
    n1 = n1->previous;
    n2 = n2->previous;
  }
  return 0;
}

// Compares two signed numbers. Returns 1 if num1 > num2, -1 if num1 < num2, and
// 0 if equal.
int compareLinkedNumbers(LinkedNumber *num1, LinkedNumber *num2) {
  int magnitude = compareMagnitudes(num1, num2);

  // Different signs: the positive one is bigger. Zero is never negative, so a
  // negative number is always smaller than zero.
  if (num1->negative != num2->negative)
    return num1->negative ? -1 : 1;
  // Same sign: negatives flip the result.
  return num1->negative ? -magnitude : magnitude;
}

// Adds |num1| + |num2| into an empty sum, ignoring signs.
void addMagnitudes(LinkedNumber *num1, LinkedNumber *num2, LinkedNumber *sum) {
  int rest = 0;
  DigitNode *n1 = num1->head;
  DigitNode *n2 = num2->head;
  // Adds digits for the length of the smaller of 2 numbers.
  for (int i = 0; i < smallerOfTwo(num1->length, num2->length); i++) {
    // Rest is carried from previous iterations to be added to the next digit.
    int digitSum = n1->digit + n2->digit + rest;
    if (digitSum >= 10) {
      rest = 1;
      digitSum %= 10;
    } else
      rest = 0;
    // Adds result to the end of the sum.
    putLast(sum, digitSum);
    n1 = n1->next;
    n2 = n2->next;
  }

  // Selects n as the longer number, or finishes after adding the last rest if
  // they're equally long.
  DigitNode *n;
  if (n1 != NULL)
    n = n1;
  else if (n2 != NULL)
    n = n2;
  else
    n = NULL;

  // Keeps adding rests for n's length.
  while (n != NULL) {
    int digitSum = n->digit + rest;
    if (digitSum >= 10) {
      rest = 1;
      digitSum %= 10;
    } else
      rest = 0;
    putLast(sum, digitSum);
    n = n->next;
  }
  if (rest > 0) {
    putLast(sum, rest);
  }
}

// Subtracts |num2| from |num1| into an empty result, ignoring signs. Requires
// |num1| >= |num2|.
void subtractMagnitudes(LinkedNumber *num1, LinkedNumber *num2,
                        LinkedNumber *result) {
  int borrow = 0;
  DigitNode *n1 = num1->head;
  DigitNode *n2 = num2->head;
  // |num1| >= |num2|, so num1 never runs out of digits before num2 does.
  while (n1 != NULL) {
    int digitDiff = n1->digit - borrow;
    if (n2 != NULL) {
      digitDiff -= n2->digit;
      n2 = n2->next;
    }
    // Borrows 10 from the next digit if the difference is negative.
    if (digitDiff < 0) {
      digitDiff += 10;
      borrow = 1;
    } else
      borrow = 0;
    putLast(result, digitDiff);
    n1 = n1->next;
  }
  // Removes leading zeros, so 100 - 99 gives 1 and not 001.
  trimLeadingZeros(result);
}

// Adds 2 signed linked numbers together.
LinkedNumber addLinkedNumbers(LinkedNumber *num1, LinkedNumber *num2) {
  LinkedNumber sum;
  initLinkedNumber(&sum);

  if (num1->negative == num2->negative) {
    // Same signs: add the sizes, and keep the sign.
    addMagnitudes(num1, num2, &sum);
    trimLeadingZeros(&sum);
    sum.negative = num1->negative;
  } else {
    // Different signs: subtract the smaller size from the bigger one, and keep
    // the sign of the number with the bigger size.
    int magnitude = compareMagnitudes(num1, num2);
    if (magnitude > 0) {
      subtractMagnitudes(num1, num2, &sum);
      sum.negative = num1->negative;
    } else if (magnitude < 0) {
      subtractMagnitudes(num2, num1, &sum);
      sum.negative = num2->negative;
    }
    // Equal sizes cancel out to an empty number, which is 0.
  }

  // Zero is never negative.
  if (sum.length == 0)
    sum.negative = false;
  return sum;
}

// Subtracts num2 from num1. Works as num1 + (-num2).
LinkedNumber subtractLinkedNumbers(LinkedNumber *num1, LinkedNumber *num2) {
  // A shallow copy that shares num2's digits, but with the opposite sign. It's
  // only read by addLinkedNumbers, so num2 itself is never modified.
  LinkedNumber flipped = *num2;
  flipped.negative = !num2->negative;
  return addLinkedNumbers(num1, &flipped);
}

// A method for printing linked numbers.
void printLinkedNumber(LinkedNumber number) {
  if (number.tail != NULL) {
    DigitNode digit = *number.tail;
    printf("\n%s%d", number.negative ? "-" : "", digit.digit);
    while (digit.previous != NULL) {
      digit = *digit.previous;
      printf("%d", digit.digit);
    }
    printf("\n");
  } else {
    printf("\n0\n");
  }
}

// Frees each DigitNode of a LinkedNumber.
void deepFreeLinkedNumber(LinkedNumber *number) {
  DigitNode *nextNode = number->head;
  DigitNode *previousNode = nextNode;
  while (nextNode != NULL) {
    nextNode = nextNode->next;
    free(previousNode);
    previousNode = nextNode;
  }
  number->length = 0;
  number->negative = false;
  number->head = NULL;
  number->tail = NULL;
}
