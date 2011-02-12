#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "storage.h"
#include "syntax.h"
/*

#include "env.h"
#include "print.h"

#include "apply.h"
list *eval_define(list *exp, list **env);
list *eval_appl(list *exp, list **env);
list *eval_assignment(list *exp, list **env);
list *eval_if(list *exp, list **env);
list *eval_sequence(list *exps, list **env);
list *eval_lambda(list *exp, list **env);
*/
/*
int truthy(list *exp) {
  if(exp->type == Symbol)
    return exp->data.symbolData != intern("#f", hashtable);
  else
    return 1;
}
*/

object_t eval(object_t exp, object_t *env) {
  object_t ret=exp;

  if(self_evaluating(exp)) {
    ret = exp;

  } else if(variable(exp)) {
    printf("variable\n");
    /*    ret = lookup_variable_value(exp->data.symbolData, *env);    */
  } else if(quoted(exp)) {
    ret = text_of_quotation(exp);

  } else if(assignment(exp)) {
    printf("assignment\n");
    /*    ret = eval_assignment(exp, env);*/

  } else if(definition(exp)) {
    printf("definition\n");
    /*    ret = eval_define(exp, env);*/

  } else if(lambda(exp)) {
    printf("lambda\n");
    /*    ret = eval_lambda(exp, env); */

  } else if(if_exp(exp)) {
    printf("if\n");
    /*    ret = eval_if(exp, env); */

  } else if(begin(exp)) {
    printf("begin\n");
    /*    ret = eval_sequence(begin_sequence(exp), env); */

  } else { // otherwise, assume it's an application
    printf("application\n");
    /*    ret = eval_appl(exp, env); */
  }

  return ret;
}
