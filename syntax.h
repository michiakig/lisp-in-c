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
object_t operands(object_t exp);
object_t operator(object_t exp);
object_t if_predicate(object_t exp);
object_t if_consequent(object_t exp);
object_t if_alternative(object_t exp);
object_t begin_sequence(object_t exp);
object_t lambda_params(object_t exp);
object_t lambda_body(object_t exp);

#endif
