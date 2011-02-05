
#ifndef __env__
#define __env__ 1

#include "list.h"
#include "eval.h"

typedef struct binding {
  char *name;
  LIST *value;
} BINDING;

LIST *lookup_variable_value(char *variable, LIST *env);
LIST *define_variable(char *var, LIST *value, LIST *env);

#endif
