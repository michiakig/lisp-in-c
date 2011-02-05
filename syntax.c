#include <string.h> /* for strcmp */
#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "eval.h"
#include "syntax.h"
#include "str_utils.h"

/* self-evaluating expressions are numbers */
int self_evaluating(LIST *exp) {
  return (exp->type == ATOM && all_digits((char*)exp->data));
}

/* variables are just symbols (any other string)*/
int variable(LIST *exp) {
  return (exp->type == ATOM && !(all_digits((char*)exp->data)));
}

/* quoted expessions start with "quote" */
int quoted(LIST *exp) {
  return tagged_list(exp, "quote");
}

int assignment(LIST *exp) {
  return tagged_list(exp, "set!");
}

int definition(LIST *exp) {
  return tagged_list(exp, "define");
}

int lambda(LIST *exp) {
  return tagged_list(exp, "lambda");
}

int if_exp(LIST *exp) {
  return tagged_list(exp, "if");
}

int begin(LIST *exp) {
  return tagged_list(exp, "begin");
}

/* check if an expression is a list starting with some tag */
int tagged_list(LIST *exp, const char *tag) {
  if(exp->type == CONS) {
    LIST *l = (LIST*)exp->data;
    if(l->type == ATOM && l->data != NULL)
      return (strcmp((char*)l->data, tag) == 0);
    else
      return 0;
  } else {
    return 0;
  }
}

LIST *text_of_quotation(LIST *exp) {
  LIST *l = exp->data;
  LIST *cdr = l->next;

  LIST *ret = malloc(sizeof(LIST));
  ret->type = ATOM;
  /* should we copy the data? if not, why malloc a new LIST? */
  ret->data = (char*)cdr->data;
  ret->next = NULL;
  return ret;
}

/* Doesn't handle (define (fn x) ... ), assumes exp is actually a define */
LIST *definition_variable(LIST *exp) {
  LIST *l = (LIST*)exp->data;
  LIST *cdr = (LIST*)l->next;

  if(cdr->type == ATOM) {
    LIST *d = malloc(sizeof(LIST));
    d->type = ATOM;
    d->data = cdr->data;
    return d;
  } else {
    return NULL;
  }
}

LIST *definition_value(LIST *exp) {
  LIST *l = (LIST*)exp->data;
  LIST *cddr = ((LIST*)l->next)->next;

  LIST *d = malloc(sizeof(LIST));
  d->data = cddr->data;

  if(cddr->type == ATOM)
    d->type = ATOM;
  else
    d->type = CONS;
  return d;
}

LIST *operator(LIST *exp) {
  return (LIST*)exp->data;
}

LIST *operands(LIST *exp) {
  LIST *l = (LIST*)exp->data;
  LIST *cdr = (LIST*)l->next;

  return cdr;
}
