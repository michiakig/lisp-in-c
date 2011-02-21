#include <stdio.h>
#include <stdlib.h>

#include "../storage.h"

int main(int argc, char **argv) {
  object_t head;

  object_t obj = obj_new_symbol(argv[1]);
  head = cons(obj, NIL);
  int i;
  for(i = 2; i < argc; i++) {
    obj = obj_new_symbol(argv[i]);
    storage_append(obj, head);
  }

  print_object(head);
  printf("\n");

  object_t nest = cons(head, cons(obj_new_symbol(argv[1]), NIL));
  print_object(nest);
  printf("\n");
}
