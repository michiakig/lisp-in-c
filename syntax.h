
#ifndef __syntax__
#define __syntax__ 1

int self_evaluating(DATA *expr);
int variable(DATA *expr);
int quoted(DATA *expr);
int tagged_list(DATA *expr, const char *tag);
int assignment(DATA *expr);
int definition(DATA *expr);
int lambda(DATA *expr);
int if_expr(DATA *expr);
int begin(DATA *expr);

DATA *text_of_quotation(DATA *expr);
DATA *definition_value(DATA *expr);
DATA *definition_variable(DATA *expr);

LLIST *operands(DATA *expr);
DATA *operator(DATA *expr);

#endif
