#ifndef syntax_included
#define syntax_included
#include "storage.h"
int self_evaluating(object_t exp);
int variable(object_t exp);
int quoted(object_t exp);
int assignment(object_t exp);
int definition(object_t exp);
int lambda(object_t exp);
int if_exp(object_t exp);
int begin(object_t exp);

object_t text_of_quotation(object_t exp);

/*
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
*/
#endif
