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

object_t DEFINE = NULL;
object_t IF = NULL;
object_t LAMBDA = NULL;
object_t SET = NULL;
object_t BEGIN = NULL;
object_t QUOTE = NULL;
object_t TRUE = NULL;
object_t FALSE = NULL;

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
  return symbolp(exp) && !self_evaluating(exp);
    
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

/* TODO: check that the special forms are of the proper arity */
object_t text_of_quotation(object_t exp) {
  return car(cdr(exp));
}


object_t definition_variable(object_t exp) {
  return car(cdr(exp));
}

object_t definition_value(object_t exp) {
  return car(cdr(cdr(exp)));
}


object_t operator(object_t exp) {
  return car(exp);
}

object_t operands(object_t exp) {
  return cdr(exp);
}


object_t if_predicate(object_t exp) {
  return car(cdr(exp));

}

object_t if_consequent(object_t exp) {
  return car(cdr(cdr(exp)));
}

object_t if_alternative(object_t exp) {
  return car(cdr(cdr(cdr(exp))));
}


object_t begin_sequence(object_t exp) {
  return cdr(exp);
}

object_t lambda_params(object_t exp) {
  return car(cdr(exp));
}

object_t lambda_body(object_t exp) {
  return cdr(cdr(exp));
}
