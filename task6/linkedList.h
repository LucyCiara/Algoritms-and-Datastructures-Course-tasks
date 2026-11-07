#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

// The node of a value.
#include <stdlib.h>
typedef struct NodeStruct {
  void *value;
  struct NodeStruct *previous;
  struct NodeStruct *next;
} Node;

// A whole linked list.
typedef struct {
  int length;
  struct NodeStruct *head;
  struct NodeStruct *tail;
} LinkedList;

// Internal method to add a new value to the list.
Node *newNode(char *value, Node *n, Node *p);

// Implements newNode to add a value to to the front.
void putFirst(LinkedList *list, char *value);

// Implements newNode to add a value to the end.
void putLast(LinkedList *list, char *value);

// Sets a LinkedList to an empty list.
void initLinkedList(LinkedList *list);

// Frees each Node of a LinkedList.
void deepFreeLinkedList(LinkedList *list);

#endif
