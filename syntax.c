#include <string.h> /* for strcmp */
#include <stdlib.h>
#include <stdio.h>

#include "llist.h"
#include "eval.h"
#include "syntax.h"
#include "str_utils.h"

/* self-evaluating expressions are numbers */
int self_evaluating(DATA *expr) {
  return (expr->type == ATOM && all_digits((char*)expr->data));
}

/* variables are just symbols (any other string)*/
int variable(DATA *expr) {
  return (expr->type == ATOM && !(self_evaluating(expr)));
}

/* quoted expressions start with "quote" */
int quoted(DATA *expr) {
  return tagged_list(expr, "quote");
}

int assignment(DATA *expr) {
  return tagged_list(expr, "set!");
}

int definition(DATA *expr) {
  return tagged_list(expr, "define");
}

int lambda(DATA *expr) {
  return tagged_list(expr, "lambda");
}

int if_expr(DATA *expr) {
  return tagged_list(expr, "if");
}

int begin(DATA *expr) {
  return tagged_list(expr, "begin");
}

/* check if an expression is a list starting with some tag */
int tagged_list(DATA *expr, const char *tag) {
  if(expr->type == LIST) {
    LLIST *l = expr->data;
    if(l->data != NULL) // empty list
      return (strcmp((char*)l->data, tag) == 0);
    else
      return 0;
  } else {
    return 0;
  }
}

DATA *text_of_quotation(DATA *expr) {
  LLIST *l = (LLIST*)expr->data;
  LLIST *n = l->next;
  DATA *d = malloc(sizeof(DATA));
  d->type = ATOM;
  d->data = (char*)n->data;

  return d;
}

DATA *definition_variable(DATA *expr) {
  LLIST *cdr = ((LLIST*)expr->data)->next;
  if(cdr->primitive == 1) {
    DATA *d = malloc(sizeof(DATA));
    d->type = ATOM;
    d->data = cdr->data;
    return d;
  } else {

  }
}

DATA *definition_value(DATA *expr) {
  LLIST *cddr = (((LLIST*)expr->data)->next)->next;
  DATA *d = malloc(sizeof(DATA));
  d->data = cddr->data;

  if(cddr->primitive == 1) {
    d->type = ATOM;
  } else {
    d->type = LIST;
  }

  return d;
}

DATA *operator(DATA *expr) {
  LLIST *car = ((LLIST*)expr->data);
  return (DATA*)car->data;
}

LLIST *operands(DATA *expr) {
  LLIST *cdr = ((LLIST*)expr->data)->next;
  return cdr;
}
