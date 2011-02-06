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

list *global_env;

char *in_prompt = ";;; C-eval input:";
char *val_prompt = ";;; C-eval value:";

void print_exp(list *exp) {
  if(exp != NULL) {
    if(exp->type == Atom) {
      printf("%s", exp->kindData.atomData);
    } else if(exp->type == Proc) {
      printf("<proc>");
    } else if(exp->type == List){
      print_sexp(exp->kindData.listData);
    }
  }
}

list *shallow_node_copy(list *orig) {
  list* copy = malloc(sizeof(list));
  copy->next = NULL;
  copy->type = orig->type;
  copy->kindData = orig->kindData;

  return copy;
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
      printf("%s\n", val_prompt);

      list *val = eval(exp, &global_env);
      
      if(val != NULL) {
        print_exp(val);
        printf("\n");
      } else {
        printf("error.\n");
      }
    }
  }
}

list *eval(list *exp, list **env) {

  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Atom, NULL);

  if(self_evaluating(exp)) {
    return exp;

  } else if(variable(exp)) {
    return lookup_variable_value(exp->kindData.atomData, *env);

  } else if(quoted(exp)) {
    return text_of_quotation(exp);

  } else if(assignment(exp)) {
    return eval_assignment(exp, env);

  } else if(definition(exp)) {
    return eval_define(exp, env);

  } else if(lambda(exp)) {
    ret->kindData.atomData = "<<lambda>>";

  } else if(if_exp(exp)) {
    ret->kindData.atomData = "<<if>>";

  } else if(begin(exp)) {
    ret->kindData.atomData = "<<begin>>";

  } else { // otherwise, assume it's an application
    return eval_appl(exp, env);
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
