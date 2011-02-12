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
object_t definition_variable(object_t);
object_t definition_value(object_t);

/*
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
