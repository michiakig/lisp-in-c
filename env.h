
#ifndef __env__
#define __env__ 1

#include "llist.h"
#include "eval.h"

typedef struct binding {
  char *name;
  DATA *value;
} BIND;

DATA *lookup_variable_value(char *variable, LLIST *env);
LLIST *define_variable(char *var, DATA *value, LLIST *env);

#endif
