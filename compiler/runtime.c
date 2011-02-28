/* A simple runtime for the compiler */
#include <stdio.h>
#include <stdlib.h>
#include "runtime.h"
#include "../interpreter/storage.h"
int pc;
object_t reg[7] = {0};
object_t stack[STACKSIZE] = {0};
object_t *sp = stack;
void push(object_t x) {
  *sp = x; sp++;
}
object_t pop() {
  sp--; return *sp;
}

extern object_t entry(void);

int obj2label(object_t obj) {
  char *num = symbol_name(obj_get_symbol(obj));
  int label = atoi(num);
  return label;
}

object_t label2obj(int label) {
  char *s = malloc(sizeof(char)*100); /* TODO: calculate the num digits */
  sprintf(s, "%d", label);
  return obj_new_symbol(s);
}

int main(int argc, char **argv) {
  printf("output:");
  print_object(entry());
  printf("\n");
}
