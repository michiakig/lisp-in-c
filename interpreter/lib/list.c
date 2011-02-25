#include <stdlib.h>
#include <stdio.h>
#include "list.h"

struct list_t *list_new(void *data) {
  struct list_t *new = malloc(sizeof(*new));
  new->next = NULL;
  new->data = data;
  return new;
}

struct list_t *list_append(void *data, struct list_t *old) {
  struct list_t *last = list_new(data);
  struct list_t *n;
  for(n = old; n->next != NULL; n = n->next)
    ;
  n->next = last;
  return last;
}

struct list_t *list_push(void *data, struct list_t *tail) {
  struct list_t *new = list_new(data);
  new->next = tail;
  return new;
}

struct list_t *list_reverse(struct list_t *lst) {
  struct list_t *this = lst, *n = lst->next, *nn;
  this->next = NULL;
  while(n != NULL) {
    nn = n->next;
    n->next = this;
    this = n;
    n = nn;
  }
  return this;
}

void list_for_each(struct list_t *lst, void (*fn) (struct list_t *)) {
  struct list_t *m, *l;
  for (m = lst; m != NULL; ) {
    l = m->next;
    fn(m);
    m = l;
  }
}

void list_print_node(struct list_t *node) {
  printf("%s", (char*)node->data);
  if(node->next != NULL)
    printf("->");
}

void list_free_node(struct list_t *node) {
  free(node);
}
