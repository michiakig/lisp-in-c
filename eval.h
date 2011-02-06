
#ifndef __eval__
#define __eval__ 1

#include "types.h"
#include "list.h"

void print_exp(list *exp);

list *eval(list *exp, list **env);
list *eval_define(list *exp, list **env);
list *eval_appl(list *exp, list **env);
list *eval_assignment(list *exp, list **env);

#endif
