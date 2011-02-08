#ifndef __env__
#define __env__ 1

#include "types.h"
#include "list.h"
#include "hasht.h"

list *init_global(struct nlist *hashtable[]);

list *lookup_variable_value(symbol *variable, list *env);
list *define_variable(symbol *var, list *value, list *env);
list *set_variable(symbol *var, list *value, list *env);

list *extend_environment(list *vars, list *vals, list *env);

#endif
