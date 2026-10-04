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

// A whole number linked list.
typedef struct {
  int length;
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

// Checks if the digit is a valid number.
bool checkValidNumber(char *number) {
  for (int i = 0; number[i] != '\0'; i++) {
    if (!isdigit(number[i])) {
      return false;
    }
  }
  return true;
}

// Creates a LinkedNumber from a string of a whole positive number.
bool createLinkedNumber(LinkedNumber *emptyLinkedNumber, char *number) {
  emptyLinkedNumber->length = 0;
  emptyLinkedNumber->head = NULL;
  emptyLinkedNumber->tail = NULL;

  if (!checkValidNumber(number))
    return false;
  for (int i = 0; number[i] != '\0'; i++) {
    // Uses character arithetic to convert from char to int.
    putFirst(emptyLinkedNumber, (number[i] - '0'));
  }
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

// Adds 2 linked numbers together.
LinkedNumber addLinkedNumbers(LinkedNumber *num1, LinkedNumber *num2) {
  LinkedNumber *sum = (LinkedNumber *)malloc(1 * sizeof(LinkedNumber));
  // Wipes itself (calloc isn't always reliable)
  sum->length = 0;
  sum->head = NULL;
  sum->tail = NULL;

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

  // Selects n as the longer number, or returns after adding the last rest if
  // they're equally long.
  DigitNode *n;
  if (n1 != NULL)
    n = n1;
  else if (n2 != NULL)
    n = n2;
  else {
    if (rest > 0) {
      putLast(sum, rest);
    }
    return *sum;
  }

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
  return *sum;
}

// A method for printing linked numbers.
void printLinkedNumber(LinkedNumber number) {
  if (number.tail != NULL) {
    DigitNode digit = *number.tail;
    printf("\n%d", digit.digit);
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
}
