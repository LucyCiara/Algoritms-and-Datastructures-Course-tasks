#include "linkedNumber.c"

int main(int argc, char *argv[]) {
  LinkedNumber number1;
  createLinkedNumber(&number1, "5720");
  LinkedNumber number2;
  createLinkedNumber(&number2, "847");
  printLinkedNumber(number1);
  printLinkedNumber(number2);
  printLinkedNumber(addLinkedNumbers(&number1, &number2));
}
