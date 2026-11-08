#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

// The node of a value.
#include <stddef.h>
#include <stdlib.h>
typedef struct NodeStruct {
  void *value;
  struct NodeStruct *previous;
  struct NodeStruct *next;
} Node;

// A whole linked list.
typedef struct {
  size_t length;
  struct NodeStruct *head;
  struct NodeStruct *tail;
} LinkedList;

// Implements newNode to add a value to to the front.
void putFirst(LinkedList *list, void *value);

// Implements newNode to add a value to the end.
void putLast(LinkedList *list, void *value);

// Sets a LinkedList to an empty list.
void initLinkedList(LinkedList *list);

// Frees each Node of a LinkedList.
void deepFreeLinkedList(LinkedList *list);

#endif
