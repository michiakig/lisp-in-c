
#ifndef __syntax__
#define __syntax__ 1

#include "list.h"

int self_evaluating(LIST *exp);
int variable(LIST *exp);
int quoted(LIST *exp);
int tagged_list(LIST *exp, const char *tag);
int assignment(LIST *exp);
int definition(LIST *exp);
int lambda(LIST *exp);
int if_exp(LIST *exp);
int begin(LIST *exp);

LIST *text_of_quotation(LIST *exp);
LIST *definition_value(LIST *exp);
LIST *definition_variable(LIST *exp);

LIST *operands(LIST *exp);
LIST *operator(LIST *exp);

#endif
