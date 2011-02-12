#include <string.h> /* for strcmp */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "str_utils.h"
#include "syntax.h"
#include "storage.h"
/*
  These are syntax predicates which determe the type of an
  expression. When reading the code here, keep in mind that the list
  *exp contains the tagged union which is the actual expression. So,
  in the following:

  list *car = exp->data.listData;
  list *cdr = exp->data.listData->next;

  car, cdr are the car and cdr of the exp.

  The syntax selectors assume the the exp passed is of the correct
  type
 */

static object_t DEFINE = NULL;
static object_t IF = NULL;
static object_t LAMBDA = NULL;
static object_t SET = NULL;
static object_t BEGIN = NULL;
static object_t QUOTE = NULL;

int tagged_list(object_t, object_t);

void init_symbols() {
  DEFINE = obj_new_symbol("define");
  IF = obj_new_symbol("if");
  LAMBDA = obj_new_symbol("lambda");
  SET = obj_new_symbol("set!");
  BEGIN = obj_new_symbol("begin");
  QUOTE = obj_new_symbol("quote");
  TRUE = obj_new_symbol("#t");
  FALSE = obj_new_symbol("#f");
}


int boolean_literal(object_t exp) {
  return symbolcmp(exp, TRUE) || symbolcmp(exp, FALSE);
}


/* self-evaluating expressions are numbers and boolean literals */
int self_evaluating(object_t exp) {
  return symbolp(exp) &&
    (all_digits(obj_symbol_name(exp)) || boolean_literal(exp));
}

/* variables are symbols which are not self-evaluating */
int variable(object_t exp) {
  return symbolp(exp) &&
    
}

int quoted(object_t exp) {
  return tagged_list(exp, QUOTE);
}

int assignment(object_t exp) {
  return tagged_list(exp, SET);
}

int definition(object_t exp) {
  return tagged_list(exp, DEFINE);
}

int lambda(object_t exp) {
  return tagged_list(exp, LAMBDA);
}

int if_exp(object_t exp) {
  return tagged_list(exp, IF);
}

int begin(object_t exp) {
  return tagged_list(exp, BEGIN);
}

/* check if an expression is a list starting with some tag */
int tagged_list(object_t exp, const object_t tag) {
  if(consp(exp)) {
    object_t a = car(exp);
    return symbolp(a) && symbolcmp(a, tag);
  } else {
    return 0;
  }
}

object_t text_of_quotation(object_t exp) {
  assert(consp(exp) && !nilp(cdr(exp)));
  return car(cdr(exp));
}

/*
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
 */
