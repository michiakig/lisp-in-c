#include <stdlib.h>
#include <stdio.h>
#include "aux.h"

/* This is an auxiliary linked list implementation for use by the
   reader. */

struct node *pop(struct node **stack) {
  struct node *ret = *stack;
  *stack = (*stack)->next;
  return ret;
}

void push(struct node *new, struct node **stack) {
  new->next = *stack;
  *stack = new;
}

struct node *append(struct node *new, struct node *lst) {
  struct node *n;
  for(n = lst; n->next != NULL; n = n->next)
    ;
  n->next = new;
  return new;
}

void for_each(struct node *n, void (*fn) (struct node*)) {
  struct node *m, *l;
  for (m = n; m != NULL; ) {
    l = m->next;
    fn(m);
    m = l;
  }
}

void print_node(struct node *n) {
  printf("%s", (char*)n->data);
  if(n->next != NULL)
    printf("->");
}

void free_node(struct node *n) {
  free(n);
}
