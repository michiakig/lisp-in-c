#ifndef __apply__
#define __apply__ 1

#include "types.h"
#include "list.h"

void init_proc(proc *p, list *params, list *body,
               list *env, list* (*fun) (list *argl));

list *primitive_add(list *argl);
list *primitive_multiply(list *argl);
list *primitive_subtract(list *argl);
list *primitive_divide(list *argl);

list *primitive_lt(list *argl);
list *primitive_gt(list *argl);
list *primitive_eq(list *argl);

list *apply(proc *p, list *argl);

#endif
