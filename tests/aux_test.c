
/* Tests for the auxiliary linked list implementation */

#include <stdio.h>
#include <stdlib.h>

#include "../aux.h"

void test_stack(int len, char **data) {
  struct node *stack = NULL;
  struct node *n, *m;
  int i;
  stack = malloc(sizeof(*stack));
  stack->next = NULL;
  stack->data = *data;
  auxfor_each(stack, &auxprint_node);
  printf("\n");
  for(i = 1; i < len; i++) {
    n = malloc(sizeof(*n));
    n->next = NULL;
    n->data = *(data+i);
    auxpush(n, &stack);

    auxfor_each(stack, &auxprint_node);
    printf("\n");
  }
  
  for(i = 0; i < len; i++) {
    n = auxpop(&stack);
    printf("%s\n", (char*)n->data);
    free(n);
  }
}

void test_auxappend(int len, char **data) {
  struct node *lst = NULL;
  struct node *n = NULL;
  int i;

  lst = malloc(sizeof(*lst));
  lst->next = NULL;
  lst->data = *data;
  for(i = 1; i < len; i++) {
    n = malloc(sizeof(*n));
    n->next = NULL;
    n->data = *(data+i);
    auxappend(n, lst);

    auxfor_each(lst, &auxprint_node);
    printf("\n");
  }
  auxfor_each(lst, &auxfree_node);
}

int main(int argc, char **argv) {
  struct node *stack = NULL;
  if(argc < 2) {
    printf("call with more args\n");
    exit(0);
  }

  test_stack(argc-1, argv+1);
  test_auxappend(argc-1, argv+1);
}

