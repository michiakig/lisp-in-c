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
  return obj_get_number(obj);
}

object_t label2obj(int label) {
  return obj_new_number(label);
}

int main(int argc, char **argv) {
  printf("output:");
  print_object(entry());
  printf("\n");
}
