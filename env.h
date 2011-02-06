#ifndef __env__
#define __env__ 1

#include "types.h"
#include "list.h"

list *init_global();
list *lookup_variable_value(char *variable, list *env);
list *define_variable(char *var, list *value, list *env);
list *set_variable(char *var, list *value, list *env);
#endif
