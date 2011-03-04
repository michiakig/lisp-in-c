#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "apply.h"
#include "syntax.h"

#include "storage.h"
#include "env.h"
#include "types.h"

object_t eval_loop(object_t, object_t *);
object_t eval_define(object_t, object_t*);
object_t eval_assignment(object_t, object_t*);
object_t eval_appl(object_t, object_t*);
object_t eval_if(object_t exp, object_t *env);
object_t eval_sequence(object_t exps, object_t *env);
object_t eval_lambda(object_t exp, object_t *env);

object_t eval(object_t exp, object_t *env) {

  if(self_evaluating(exp))
    return exp;

  else if(variable(exp)) {
    object_t val = lookup_variable(exp, *env);
    if(val == NULL) {
      printf("ERROR undefined variable: %s\n", symbol_name(obj_get_symbol(exp)));
      return NULL;
    } else
      return val;
    
  } else if(quoted(exp))
    return text_of_quotation(exp);

  else if(assignment(exp))
    return eval_assignment(exp, env);

  else if(definition(exp))
    return eval_define(exp, env);

  else if(lambda(exp))
    return eval_lambda(exp, env);

  else if(if_exp(exp))
    return eval_if(exp, env);

  else if(begin(exp))
    return eval_sequence(begin_sequence(exp), env);

  else
    return eval_appl(exp, env);
}

object_t eval_define(object_t exp, object_t *env) {
  object_t var = definition_variable(exp);
  object_t val = definition_value(exp);
  val = eval(val, env);
  if(val == NULL)
    return NULL;
  *env = define_variable(var, val, *env);
  return obj_new_symbol("ok");
}

object_t eval_assignment(object_t exp, object_t *env) {
  object_t var = definition_variable(exp);
  object_t val = eval(definition_value(exp), env);
  if(val == NULL)
    return NULL;
  *env = set_variable(var, val, *env);
  return obj_new_symbol("ok");
}

object_t eval_appl(object_t exp, object_t *env) {

  object_t op = operator(exp);
  object_t ev_op = eval(op, env);
  if(ev_op == NULL)
    return NULL;

  object_t opands = operands(exp);
  if(isnull(opands))
    return apply(ev_op, NIL);

  object_t opand = car(opands);
  object_t ev_opand = eval(opand, env);
  if(ev_opand == NULL)
    return NULL;

  object_t ev_opands = cons(ev_opand, NIL);
  opands = cdr(opands);

  while(!isnull(opands)) {
    ev_opand = eval(car(opands), env);
    if(ev_opand == NULL)
      return NULL;
    storage_append(ev_opand, ev_opands);
    opands = cdr(opands);
  }

  return apply(ev_op, ev_opands);
}

int truthy(object_t exp) {
  if(issymbol(exp))
    return !obj_symbol_cmp(exp, obj_new_symbol("#f"));
  else
    return 1;
}

object_t eval_if(object_t exp, object_t *env) {
  object_t pred = if_predicate(exp);
  object_t consq = if_consequent(exp);
  object_t alt = if_alternative(exp);
  object_t ev_pred = eval(pred, env);
  if(truthy(ev_pred))
    return eval(consq, env);
  else
    return eval(alt, env);
}

object_t eval_sequence(object_t exps, object_t *env) {
  while(!isnull(cdr(exps))) {
    eval(car(exps), env);
    exps = cdr(exps);
  }
  return eval(car(exps), env);
}

object_t eval_lambda(object_t exp, object_t *env) {

  return obj_new_compound(car(cdr(exp)), cdr(cdr(exp)), *env);
}

