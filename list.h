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

void simple_rfree(list *l);

list *shallow_node_copy(list *orig);

symbol *getSymbol(list *l);
list *getList(list *l);
procedure *getProc(list *l);
binding *getBind(list *l);

#endif
