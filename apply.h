
#ifndef __apply__
#define __apply__ 1

#include "eval.h"
#include "llist.h"

typedef struct procedure {
  //  enum datatype type;
  LIST *params;
  LIST *body;
  LIST *env;
  LIST* (*fun) (LIST *argl);
} PROCEDURE;

LIST *primitive_add(LIST *argl);
LIST *apply(PROCEDURE *proc, LIST *argl);

#endif
