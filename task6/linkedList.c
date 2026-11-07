#include <stdbool.h>
#include <stdlib.h>

// The node of a value.
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
Node *newNode(char *value, Node *n, Node *p) {
  Node *newValue = (Node *)(malloc(1 * sizeof(Node)));
  newValue->value = value;
  newValue->next = n;
  newValue->previous = p;
  return newValue;
}

// Implements newNode to add a value to to the front.
void putFirst(LinkedList *list, char *value) {
  // Adds new list with a given value, and the old head as the next in the
  // link.
  Node *new = newNode(value, list->head, NULL);

  // Sets itself as the new head, and as the tail if there is none. Sets the old
  // head's previous as itself.
  list->head = new;
  if (!list->tail)
    list->tail = new;
  else
    new->next->previous = new;
  list->length++;
}

// Implements newNode to add a value to the end.
void putLast(LinkedList *list, char *value) {
  // Adds new list with a given value, and the old tail as the previous in the
  // link.
  Node *new = newNode(value, NULL, list->tail);

  // Sets itself as the new tail, and the head if there is none. Sets the old
  // tail's next as itself.
  list->tail = new;
  if (!list->head)
    list->head = new;
  else
    new->previous->next = new;
  list->length++;
}

// Sets a LinkedList to an empty list.
void initLinkedList(LinkedList *list) {
  list->length = 0;
  list->head = NULL;
  list->tail = NULL;
}

// Frees each Node of a LinkedList.
void deepFreeLinkedList(LinkedList *list) {
  Node *nextNode = list->head;
  Node *previousNode = nextNode;
  while (nextNode != NULL) {
    nextNode = nextNode->next;
    free(previousNode);
    previousNode = nextNode;
  }
  list->length = 0;
  list->head = NULL;
  list->tail = NULL;
}
