#include <stdlib.h>

typedef struct DigitNodeStruct {
  int digit;
  struct DigitNodeStruct *previous;
  struct DigitNodeStruct *next;
} DigitNode;

typedef struct {
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
  if (new->next)
    new->next->previous = new;
  else
    number->tail = new;
  number->head = new;
}

void createLinkedNumber(LinkedNumber *emptyLinkedNumber, char *number) {
  for (int i = 0; number[i] != '\0'; i++) {
    putFirst(emptyLinkedNumber, (number[i] - '0'));
  }
}
