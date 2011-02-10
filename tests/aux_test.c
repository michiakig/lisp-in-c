
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
  for_each(stack, &print_node);
  printf("\n");
  for(i = 1; i < len; i++) {
    n = malloc(sizeof(*n));
    n->next = NULL;
    n->data = *(data+i);
    push(n, &stack);

    for_each(stack, &print_node);
    printf("\n");
  }
  
  for(i = 0; i < len; i++) {
    n = pop(&stack);
    printf("%s\n", (char*)n->data);
    free(n);
  }
}

void test_append(int len, char **data) {
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
    append(n, lst);

    for_each(lst, &print_node);
    printf("\n");
  }
  for_each(lst, &free_node);
}

int main(int argc, char **argv) {
  struct node *stack = NULL;
  if(argc < 2) {
    printf("call with more args\n");
    exit(0);
  }

  test_stack(argc-1, argv+1);
  test_append(argc-1, argv+1);
}

