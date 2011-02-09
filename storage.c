#include <stdio.h>
#include "list.h"
#include "types.h"
#include "storage.h"
#include "assert.h"
#include "print.h"

#define MEMORY 10

list *the_cars[MEMORY];
list *the_cdrs[MEMORY];

list *new_cars[MEMORY];
list *new_cdrs[MEMORY];

static int free = 0;

int cons(list *a, list *d) {
  if(free < MEMORY) {
    the_cars[free] = shallow_node_copy(a);
    the_cdrs[free] = shallow_node_copy(d);
    int ret = free;
    free++;
    return ret;
  } else {
    // gc
  }
}

list *car(int i) {
  assert(i < MEMORY);
  return the_cars[i];
}

list *cdr(int i) {
  assert(i < MEMORY);
  return the_cdrs[i];
}

void print_cons(list *c) {
  assert(c->type == Cons);
  int i = *(c->data.consData);
  assert(i < MEMORY);
  printf("(");
  print_data(the_cars[i]);
  printf(" . ");
  print_data(the_cdrs[i]);
  printf(")");
}
