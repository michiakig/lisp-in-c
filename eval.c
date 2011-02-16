#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "storage.h"
#include "syntax.h"
#include "env.h"
#include "types.h"
#include "apply.h"

object_t eval_define(object_t, object_t*);
object_t eval_assignment(object_t, object_t*);
object_t eval_appl(object_t, object_t*);
object_t eval_if(object_t exp, object_t *env);
object_t eval_sequence(object_t exps, object_t *env);
object_t eval_lambda(object_t exp, object_t *env);

object_t eval(object_t exp, object_t *env) {
  if(self_evaluating(exp)) {
    return exp;

  } else if(variable(exp)) {
    return lookup_variable(exp, *env);

  } else if(quoted(exp)) {
    return text_of_quotation(exp);

  } else if(assignment(exp)) {
    return eval_assignment(exp, env);

  } else if(definition(exp)) {
    return eval_define(exp, env);

  } else if(lambda(exp)) {
    return eval_lambda(exp, env);

  } else if(if_exp(exp)) {
    return eval_if(exp, env);

  } else if(begin(exp)) {
    return eval_sequence(begin_sequence(exp), env);

  } else {
    return eval_appl(exp, env);
  }
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

object_t eval_appl(object_t exp, object_t *env) {
  object_t op = operator(exp);

  object_t ev_op = eval(op, env);
  object_t opands = operands(exp);
  object_t opand = car(opands);
  object_t ev_opands = cons(eval(opand, env), NIL);

  opands = cdr(opands);
  while(!nilp(opands)) {
    storage_append(eval(car(opands), env), ev_opands);
    opands = cdr(opands);
  }

  return apply(ev_op, ev_opands);
}

int truthy(object_t exp) {
  if(symbolp(exp))
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
  while(!nilp(cdr(exps))) {
    eval(car(exps), env);
    exps = cdr(exps);
  }
  return eval(car(exps), env);
}

object_t eval_lambda(object_t exp, object_t *env) {
  return cons(obj_new_symbol("_lambda"),
              cons(*env, cdr(exp)));
}
