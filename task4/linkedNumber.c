#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct DigitNodeStruct {
  int digit;
  struct DigitNodeStruct *previous;
  struct DigitNodeStruct *next;
} DigitNode;

typedef struct {
  int length;
  struct DigitNodeStruct *head;
  struct DigitNodeStruct *tail;
} LinkedNumber;

DigitNode *newNode(int digit, DigitNode *n, DigitNode *p) {
  DigitNode *newDigit = (DigitNode *)(malloc(1 * sizeof(DigitNode)));
  newDigit->digit = digit;
  newDigit->next = n;
  newDigit->previous = p;
  return newDigit;
}

void putFirst(LinkedNumber *number, int digit) {
  DigitNode *new = newNode(digit, number->head, NULL);
  number->head = new;
  if (!number->tail)
    number->tail = new;
  else
    new->next->previous = new;
  number->length++;
}

void putLast(LinkedNumber *number, int digit) {
  DigitNode *new = newNode(digit, NULL, number->tail);
  number->tail = new;
  if (!number->head)
    number->head = new;
  else
    new->previous->next = new;
  number->length++;
}

bool checkValidNumber(char *number) {
  for (int i = 0; number[i] != '\0'; i++) {
    if (!isdigit(number[i])) {
      return false;
    }
  }
  return true;
}

bool createLinkedNumber(LinkedNumber *emptyLinkedNumber, char *number) {
  emptyLinkedNumber->length = 0;
  emptyLinkedNumber->head = NULL;
  emptyLinkedNumber->tail = NULL;

  if (!checkValidNumber(number))
    return false;
  for (int i = 0; number[i] != '\0'; i++) {
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

LinkedNumber addLinkedNumbers(LinkedNumber *num1, LinkedNumber *num2) {
  LinkedNumber *sum = (LinkedNumber *)malloc(1 * sizeof(LinkedNumber));
  sum->length = 0;
  sum->head = NULL;
  sum->tail = NULL;

  int rest = 0;
  DigitNode *n1 = num1->head;
  DigitNode *n2 = num2->head;
  for (int i = 0; i < smallerOfTwo(num1->length, num2->length); i++) {
    int digitSum = n1->digit + n2->digit + rest;
    if (digitSum >= 10) {
      rest = 1;
      digitSum %= 10;
    } else
      rest = 0;
    putLast(sum, digitSum);
    n1 = n1->next;
    n2 = n2->next;
  }

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

void deepFreeLinkedNumber(LinkedNumber *number) {
  DigitNode *nextNode = number->head;
  DigitNode *previousNode = nextNode;
  while (nextNode != NULL) {
    nextNode = nextNode->next;
    free(previousNode);
    previousNode = nextNode;
  }
}
