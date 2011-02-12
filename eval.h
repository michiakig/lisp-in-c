
#ifndef __eval__
#define __eval__ 1

#include "types.h"
#include "list.h"

list *eval(list *exp, list **env);
list *eval_define(list *exp, list **env);
list *eval_appl(list *exp, list **env);
list *eval_assignment(list *exp, list **env);
list *eval_if(list *exp, list **env);
list *eval_sequence(list *exps, list **env);
list *eval_lambda(list *exp, list **env);

#endif
