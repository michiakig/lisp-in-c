#include <string.h> /* for strcmp */
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "list.h"
#include "str_utils.h"

#include "syntax.h"
#include "eval.h"

/*
  These are syntax predicates which determe the type of an
  expression. When reading the code here, keep in mind that the list
  *exp contains the tagged union which is the actual expression. So,
  in the following:

  list *car = exp->kindData.listData;
  list *cdr = exp->kindData.listData->next;

  car, cdr are the car and cdr of the exp.

  The syntax selectors assume the the exp passed is of the correct type
 */

/* self-evaluating expressions are just numbers */
int self_evaluating(list *exp) {
  return exp->type == Atom && all_digits(exp->kindData.atomData);
}

/* variables are just symbols (any other string) */
int variable(list *exp) {
  return exp->type == Atom &&
    (!all_digits(exp->kindData.atomData) || strcmp
}

/* quoted expessions start with "quote" */
int quoted(list *exp) {
  return tagged_list(exp, "quote");
}

int assignment(list *exp) {
  return tagged_list(exp, "set!");
}

int definition(list *exp) {
  return tagged_list(exp, "define");
}

int lambda(list *exp) {
  return tagged_list(exp, "lambda");
}

int if_exp(list *exp) {
  return tagged_list(exp, "if");
}

int begin(list *exp) {
  return tagged_list(exp, "begin");
}

/* check if an expression is a list starting with some tag */
int tagged_list(list *exp, const char *tag) {
  if(exp->type == List) {
    list *car = exp->kindData.listData;
    if(car->type == Atom && car->kindData.atomData != NULL)
      return strcmp(car->kindData.atomData, tag) == 0;
    else
      return 0;
  } else {
    return 0;
  }
}

list *text_of_quotation(list *exp) {
  list *cdr = exp->kindData.listData->next;
  /* TODO: copy the data? if not, why malloc a new list? */
  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Atom, cdr->kindData.atomData);
  return ret;
}

/* TODO: handle (define (fn x) ... ) */
list *definition_variable(list *exp) {
  list *cdr = exp->kindData.listData->next;
  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Atom, cdr->kindData.atomData);
  return ret;
}

list *definition_value(list *exp) {
  list *cddr = exp->kindData.listData->next->next;

  list *ret = malloc(sizeof(list));
  if(cddr->type == Atom) // value could be an atom or a list
    init_list(ret, NULL, Atom, cddr->kindData.atomData);
  else
    init_list(ret, NULL, List, cddr->kindData.listData);    
  return ret;
}

list *operator(list *exp) {
  list *car = exp->kindData.listData;
  list *copy = malloc(sizeof(list));
  if(car->type == List)
    init_list(copy, NULL, List, car->kindData.listData);
  else if(car->type == Atom)
    init_list(copy, NULL, Atom, car->kindData.atomData);
  return copy;
}

list *operands(list *exp) {
  list *cdr = exp->kindData.listData->next;
  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, List, cdr);    
  return ret;  
}
