
#include <stdio.h>
#include "../list.h"

int main(int argc, char **argv) {
  int i;
  struct list_t *head = NULL;
  for(i = 1; i < argc; i++) {
    head = list_push(argv[i], head);
  }

  list_for_each(head, &list_print_node);
  printf("\n");
  head = list_reverse(head);
  list_for_each(head, &list_print_node);
  printf("\n");
}
