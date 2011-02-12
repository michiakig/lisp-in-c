#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "storage.h"
#include "syntax.h"
#include "env.h"

object_t eval_define(object_t exp, object_t *env);
object_t eval_assignment(object_t exp, object_t *env);

/*
#include "print.h"

#include "apply.h"

list *eval_appl(list *exp, list **env);

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
  object_t ret = exp;

  if(self_evaluating(exp)) {
    ret = exp;

  } else if(variable(exp)) {
    ret = lookup_variable(exp, *env);

  } else if(quoted(exp)) {
    ret = text_of_quotation(exp);

  } else if(assignment(exp)) {
    ret = eval_assignment(exp, env);

  } else if(definition(exp)) {
    ret = eval_define(exp, env);

  } else if(lambda(exp)) {
    printf("lambda\n");
    /*    ret = eval_lambda(exp, env); */

  } else if(if_exp(exp)) {
    printf("if\n");
    /*    ret = eval_if(exp, env); */

  } else if(begin(exp)) {
    printf("begin\n");
    /*    ret = eval_sequence(begin_sequence(exp), env); */

  } else { /* otherwise, assume it's an application */
    printf("application\n");
    /*    ret = eval_appl(exp, env); */
  }

  return ret;
}

object_t eval_define(object_t exp, object_t *env) {
  object_t var = definition_variable(exp);
  object_t val = definition_value(exp);
  val = eval(val, env);
  *env = define_variable(var, val, *env);
  return obj_new_symbol("ok");
}

object_t eval_assignment(object_t exp, object_t *env) {
  object_t var = definition_variable(exp);
  object_t val = eval(definition_value(exp), env);
  *env = set_variable(var, val, *env);
  return obj_new_symbol("ok");
}

/*
list *eval_appl(list *exp, list **env) {

  list *op = operator(exp);
  list *ev_op = eval(op, env);
  free(op);


  list *opands = operands(exp);
  list *opands_data = getList(opands);
  free(opands);
  list *ev_opands = NULL;

  for(list *n = opands_data; n != NULL; n = n->next) {

    list *copy = shallow_node_copy(n);
    list *ev_opand = eval(copy, env);
    if(copy != ev_opand)
      free(copy);

    list *arg = malloc(sizeof(list));
    init_list(arg, NULL, List, ev_opand);    
    if(ev_opands == NULL) {definition_variabdefinition_value      ev_opands = arg;
    } else {
      append(arg, ev_opands);
    }
  }

  list *ret = apply(getProc(ev_op), ev_opands);
  simple_rfree(ev_opands);
  free(ev_op);

  return ret;
}

list *eval_if(list *exp, list **env) {
  list *pred = if_predicate(exp);
  list *consq = if_consequent(exp);
  list *alt = if_alternative(exp);
  list *ev_pred = eval(pred, env);
  if(truthy(ev_pred)) {
    return eval(consq, env);
  } else {
    return eval(alt, env);
  }
}

list *eval_sequence(list *exps, list **env) {
  list *n;

  for(n = exps; n->next != NULL; n = n->next) {
    list *copy = shallow_node_copy(n);
    eval(copy, env);
    free(copy);
  }

  list *ret = eval(n, env);
  return ret;
}

list *eval_lambda(list *exp, list **env) {
  procedure *lambda = malloc(sizeof(procedure));
  list *params = lambda_params(exp);
  list *body = lambda_body(exp);
  init_proc(lambda, params->data.listData, body->data.listData, *env, NULL);
  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Procedure, lambda);
  return ret;
}

*/
