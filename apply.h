
#ifndef __apply__
#define __apply__ 1

#include "eval.h"
#include "llist.h"

typedef struct procedure {
  //  enum datatype type;
  LLIST *params;
  LLIST *body;
  LLIST *env;
  DATA* (*fun) (LLIST *argl);
} PROCEDURE;

DATA *primitive_add(LLIST *argl);
DATA *apply(PROCEDURE *proc, LLIST *argl);

#endif
