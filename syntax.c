#include <string.h> /* for strcmp */
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "list.h"
#include "str_utils.h"

#include "syntax.h"
#include "eval.h"

#include "hasht.h"

/*
  These are syntax predicates which determe the type of an
  expression. When reading the code here, keep in mind that the list
  *exp contains the tagged union which is the actual expression. So,
  in the following:

  list *car = exp->data.listData;
  list *cdr = exp->data.listData->next;

  car, cdr are the car and cdr of the exp.

  The syntax selectors assume the the exp passed is of the correct type
 */

static symbol *DEFINE;
static symbol *IF;
static symbol *LAMBDA;
static symbol *SET;
static symbol *BEGIN;
static symbol *QUOTE;

void init_hashtable(struct nlist *hashtable[], int len) {
  DEFINE = intern("define", hashtable);
  IF = intern("if", hashtable);
  LAMBDA = intern("lambda", hashtable);
  SET = intern("set!", hashtable);
  BEGIN = intern("begin", hashtable);
  QUOTE = intern("quote", hashtable);
}

int boolean_literal(char *str) {
  return (strcmp(str, "#f") == 0)
    || (strcmp(str, "#t") == 0)
    || (strcmp(str, "#T") == 0)
    || (strcmp(str, "#F") == 0);
}

/* self-evaluating expressions are numbers and boolean literals */
int self_evaluating(list *exp) {
  return exp->type == Symbol && (all_digits(exp->data.symbolData->name)
                               || boolean_literal(exp->data.symbolData->name));
}

/* variables are just symbols (any other string) */
int variable(list *exp) {
  return exp->type == Symbol && !all_digits(exp->data.symbolData->name)
    && exp->data.symbolData->name[0] != '#';
}

/* quoted expessions start with "quote" */
int quoted(list *exp) {
  return tagged_list(exp, QUOTE);
}

int assignment(list *exp) {
  return tagged_list(exp, SET);
}

int definition(list *exp) {
  return tagged_list(exp, DEFINE);
}

int lambda(list *exp) {
  return tagged_list(exp, LAMBDA);
}

int if_exp(list *exp) {
  return tagged_list(exp, IF);
}

int begin(list *exp) {
  return tagged_list(exp, BEGIN);
}

/* check if an expression is a list starting with some tag */
int tagged_list(list *exp, const symbol *tag) {
  if(exp->type == List) {
    list *car = exp->data.listData;
    return car->type == Symbol && car->data.symbolData == tag;
  } else {
    return 0;
  }
}
/* TODO: doesn't handle (quote (foo bar)) */
list *text_of_quotation(list *exp) {
  list *cdr = exp->data.listData->next;

  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Symbol, cdr->data.symbolData);
  return ret;
}

list *definition_variable(list *exp) {
  list *cdr = exp->data.listData->next;

  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Symbol, cdr->data.symbolData);
  return ret;
}

list *definition_value(list *exp) {
  list *cddr = exp->data.listData->next->next;

  list *ret = malloc(sizeof(list));
  if(cddr->type == Symbol) { // value could be a symbol or a list
    init_list(ret, NULL, Symbol, cddr->data.symbolData);
  } else {
    init_list(ret, NULL, List, cddr->data.listData);
  }
  return ret;
}

list *operator(list *exp) {
  list *car = exp->data.listData;

  list *ret = malloc(sizeof(list));
  if(car->type == Symbol)
    init_list(ret, NULL, Symbol, car->data.symbolData);
  else if(car->type == List)
    init_list(ret, NULL, List, car->data.listData);
  return ret;
}

list *operands(list *exp) {
  list *cdr = exp->data.listData->next;

  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, List, cdr);
  return ret;  
}

list *if_predicate(list *exp) {
  list *l = exp->data.listData;
  list *pred = l->next;
  return pred;
}

list *if_consequent(list *exp) {
  list *l = exp->data.listData;
  list *consq = l->next->next;
  return consq;
}

list *if_alternative(list *exp) {
  list *l = exp->data.listData;
  list *alt = l->next->next->next;
  return alt;
}

list *begin_sequence(list *exp) {
  list *l = exp->data.listData;
  list *cdr = l->next;

  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, List, cdr);
  return ret;
}

list *lambda_params(list *exp) {
  list *l = exp->data.listData;
  list *cdr = l->next;

  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, List, deep_list_copy(getList(cdr)));
  return ret;
}

list *lambda_body(list *exp) {
  list *l = exp->data.listData;
  list *cddr = l->next->next;

  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, List, deep_list_copy(cddr));
  return ret;
}
