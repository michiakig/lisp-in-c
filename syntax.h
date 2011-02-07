#ifndef __syntax__
#define __syntax__ 1

#include "types.h"
#include "list.h"

int self_evaluating(list *exp);
int variable(list *exp);
int quoted(list *exp);
int tagged_list(list *exp, const char *tag);
int assignment(list *exp);
int definition(list *exp);
int lambda(list *exp);
int if_exp(list *exp);
int begin(list *exp);

list *text_of_quotation(list *exp);
list *definition_value(list *exp);
list *definition_variable(list *exp);

list *operands(list *exp);
list *operator(list *exp);

list *if_predicate(list *exp);
list *if_consequent(list *exp);
list *if_alternative(list *exp);

list *begin_sequence(list *exp);
list *lambda_params(list *exp);
list *lambda_body(list *exp);

#endif
