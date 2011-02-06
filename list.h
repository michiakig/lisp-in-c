#ifndef __list__
#define __list__ 1

#include "types.h"

/*
  Linked-list implementation containing additional information about
  the type of data held in each node.

  I've chosen this implementation so that a list node can serve as
  part of a larger list and as a basic data wrapper which can contain
  either a list or atomic data.

  See http://en.wikipedia.org/wiki/Tagged_union for more details
*/

void init_list(list *node, list *next, enum kind type, void *data);

list *cons(list *new, list *old);
list *insert(list *new, list *old);
list *append(list *new, list *old);

list *push(list *new, list **pstack);
list *pop(list **pstack);

void print_sexp(list *head);
void print_boxp(list *head);
void simple_rfree(list *l);

#endif
