#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "list.h"
#include "str_utils.h"

#include "eval.h"
#include "apply.h"
#include "read.h"
#include "syntax.h"
#include "env.h"
#include "print.h"

list *global_env;

char *in_prompt = ";;; C-eval input:";
char *val_prompt = ";;; C-eval value:";

int truthy(list *exp) {
  if(exp->type == Atom)
    return !(strcmp(exp->kindData.atomData, "#f") == 0);
  else
    return 1;
}

int main(int argc, char **argv) {
  /* initialize global environment */
  global_env = init_global();

  printf(";;; type \"q\" to quit\n");
  list *input = NULL;
  while(1) {
    printf("%s ", in_prompt);
    input = read_sexp(stdin);

    if(input != NULL) {

      if(input->type == Atom && input->kindData.atomData != NULL &&
         strcmp(input->kindData.atomData, "q\n") == 0) {
        exit(0);
      }
      list *exp = parse_sexp(input);
      simple_rfree(input);

      list *val = eval(exp, &global_env);
      
      if(val != NULL) {
        printf("%s\n", val_prompt);
        print_exp(val);
        printf("\n");
      } 
    }
  }
}

list *eval(list *exp, list **env) {
  list *ret;

  if(self_evaluating(exp)) {
    ret = exp;

  } else if(variable(exp)) {
    ret = lookup_variable_value(exp->kindData.atomData, *env);

  } else if(quoted(exp)) {
    ret = text_of_quotation(exp);

  } else if(assignment(exp)) {
    ret = eval_assignment(exp, env);

  } else if(definition(exp)) {
    ret = eval_define(exp, env);

  } else if(lambda(exp)) {
    ret = eval_lambda(exp, env);

  } else if(if_exp(exp)) {
    ret = eval_if(exp, env);

  } else if(begin(exp)) {
    ret = eval_sequence(begin_sequence(exp), env);

  } else { // otherwise, assume it's an application
    ret = eval_appl(exp, env);
  }

  return ret;
}

list *eval_define(list *exp, list **env) {
  list *var = definition_variable(exp);
  list *val = eval(definition_value(exp), env);

  // TODO: assert that var is an Atom?
  /* mutates the env */
  *env = define_variable(var->kindData.atomData, val, *env);

  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Atom, "ok");
  
  // TODO: what can we free here?

  return ret;
}

list *eval_assignment(list *exp, list **env) {
  list *var = definition_variable(exp);
  list *val = eval(definition_value(exp), env);

  *env = set_variable(var->kindData.atomData, val, *env);

  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Atom, "ok");
  return ret;
}

list *eval_appl(list *exp, list **env) {
  /* eval the operator */
  list *op = operator(exp);
  list *ev_op = eval(op, env);

  /* eval the operands */
  list *opands = operands(exp);

  list *opands_data = opands->kindData.listData;
  free(opands);
  list *ev_opands = NULL;

  for(list *n = opands_data; n != NULL; n = n->next) {

    list *copy = shallow_node_copy(n);
    list *ev_opand = eval(copy, env);

    list *arg = malloc(sizeof(list));
    init_list(arg, NULL, List, ev_opand);    
    if(ev_opands == NULL) {
      ev_opands = arg;
    } else {
      append(arg, ev_opands);
    }
  }

  /* apply the operator to the operands */
  return apply(ev_op->kindData.procData, ev_opands);
}

list *eval_if(list *exp, list **env) {
  list *pred = if_predicate(exp);
  list *consq = if_consequent(exp);
  list *alt = if_alternative(exp);

  list *ev_pred = eval(pred, env);

  if(truthy(ev_pred))
    return eval(consq, env);
  else
    return eval(alt, env);
}

list *eval_sequence(list *exps, list **env) {
  list *n;
  /* for each but the last expression */
  for(n = exps; n->next != NULL; n = n->next) {
    list *copy = shallow_node_copy(n);
    eval(copy, env);
    free(copy);
  }

  list *ret = eval(n, env);
  return ret;
}

list *eval_lambda(list *exp, list **env) {
  proc *lambda = malloc(sizeof(proc));
  list *params = lambda_params(exp);
  list *body = lambda_body(exp);
  init_proc(lambda, params->kindData.listData, body->kindData.listData, *env, NULL);
  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Proc, lambda);
  return ret;
}
