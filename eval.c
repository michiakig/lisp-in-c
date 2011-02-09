#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "list.h"
#include "str_utils.h"

#include "eval.h"
#include "read.h"
#include "hasht.h"

#include "syntax.h"
#include "env.h"
#include "print.h"

#include "apply.h"

list *global_env;

char *in_prompt = ";;; C-eval input:";
char *val_prompt = ";;; C-eval value:";

struct nlist *hashtable[HASHSIZE];

int truthy(list *exp) {
  if(exp->type == Symbol)
    return exp->data.symbolData != intern("#f", hashtable);
  else
    return 1;
}

int main(int argc, char **argv) {
  /* initialize global environment */
  global_env = init_global(hashtable);
  
  init_hashtable(hashtable, HASHSIZE);

  printf(";;; type \"q\" to quit\n");
  list *input = NULL;
  while(1) {
    printf("%s ", in_prompt);
    input = read_sexp(stdin);

    if(input != NULL) {

      if(input->type == String && input->data.stringData != NULL &&
         strcmp(input->data.stringData, "q\n") == 0) {
        exit(0);
      }

      list *exp = parse_sexp(input, hashtable);
      simple_rfree(input);

      list *val = eval(exp, &global_env);
      simple_rfree(exp); /* free the whole expression -- anything
                            that needs to be kept after evaluation
                            (lambda body and params) is copied during
                            eval process */

      if(val != NULL) {
        printf("%s\n", val_prompt);
        print_exp(val);
        
        free(val); /* only free the container list which was passed
                      back from eval */
        printf("\n");
      } else {
        printf("error? val was null\n");
      }
    }
  }
  simple_rfree(global_env);
  free_hasht(hashtable);
}

list *eval(list *exp, list **env) {
  list *ret;

  if(self_evaluating(exp)) {
    ret = exp;

  } else if(variable(exp)) {
    ret = lookup_variable_value(exp->data.symbolData, *env);

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
  list *dfn_val = definition_value(exp);
  list *val = eval(dfn_val, env);
  if(val != dfn_val)
    free(dfn_val);

  list *new_env = define_variable(getSymbol(var), val, *env);
  free(var);
  if(new_env != NULL) {
    /* mutate the env */
    *env = new_env;

    list *ret = malloc(sizeof(list));
    init_list(ret, NULL, Symbol,intern("ok", hashtable));
    return ret;
  } else {
    return NULL;
  }
}

list *eval_assignment(list *exp, list **env) {
  list *var = definition_variable(exp);
  list *val = eval(definition_value(exp), env);

  *env = set_variable(getSymbol(var), val, *env);
  free(var);

  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Symbol, intern("ok", hashtable));
  return ret;
}

list *eval_appl(list *exp, list **env) {
  /* eval the operator */
  list *op = operator(exp);
  list *ev_op = eval(op, env);
  free(op);

  /* eval the operands */
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
    if(ev_opands == NULL) {
      ev_opands = arg;
    } else {
      append(arg, ev_opands);
    }
  }

  /* apply the operator to the operands */
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
  procedure *lambda = malloc(sizeof(procedure));
  list *params = lambda_params(exp);
  list *body = lambda_body(exp);
  init_proc(lambda, params->data.listData, body->data.listData, *env, NULL);
  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Procedure, lambda);
  return ret;
}
