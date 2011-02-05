#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "env.h"
#include "list.h"
#include "str_utils.h"
#include "read.h"
#include "syntax.h"

LIST *stack;
LIST *expr;
LIST *global_env;

LIST cont;
LIST proc;
LIST *argl;
LIST *unev;

char *in_prompt = ";;; C-eval input:";
char *val_prompt = ";;; C-eval value:";

//LIST *init_env();
//
//LIST *init_env() {
//  LIST *env = malloc(sizeof(LIST));
//  env->data = NULL;
//  env->primitive = 0;
//  env->next = NULL;
//
//  LIST *frame = malloc(sizeof(LIST));
//
//  PROCEDURE *sum_proc = malloc(sizeof(PROCEDURE));
//  sum_proc->env = NULL;
//  sum_proc->params = NULL;
//  sum_proc->body = NULL;
//  sum_proc->fun = primitive_add;
//  
//  LIST *d = malloc(sizeof(LIST));
//  d->type = PROC;
//  d->data = sum_proc;
//
//  BIND *b = malloc(sizeof(BIND));
//  b->name = "plus";
//  b->value = d;
//
//  frame->data = b;
//  frame->next = NULL;
//
//  env->data = frame;
//
//  return env;
//}
//

int main(int argc, char **argv) {
  /* initialize global environment */
  //  global_env = init_env();
  global_env = malloc(sizeof(LIST));

  printf(";;; type \"quit\" to quit\n");
  LIST *input = NULL;
  while(1) {
    printf("%s ", in_prompt);
    input = read_sexp(stdin);

    if(input != NULL) {

      if(input->data != NULL &&
         (strcmp((char*)input->data, "quit\n") == 0))
        exit(0);

      LIST *exp = parse_sexp(input);
      free_r(input);

      printf("%s\n", val_prompt);

      //      LIST *val = eval(exp, &global_env);

      //      printf("%s\n", (char*)val->data);
//      if(val != NULL) {
//        print_data(val);
//        printf("\n");
//      } else {
//        printf("error.\n");
//      }

      //      free(exp->data);
      //      free(exp);
    }
  }
}

void print_data(LIST *d) {
  if(d != NULL) {
    if(d->data != NULL) {
      if(d->type == ATOM) {
        printf("%s", (char*)d->data);
      } else if(d->type == PROC) {
        printf("<proc ...>");
      } else {
        print_sexp((LIST*)d->data);
      }
    }
  }
}

LIST *eval(LIST *exp, LIST **env) {
  LIST *ret = malloc(sizeof(LIST));
  ret->next = NULL;
  ret->type = ATOM;
  ret->data;

  if(self_evaluating(exp)) {
    return exp;
  } else if(variable(exp)) {
    return lookup_variable_value((char*)exp->data, *env);
  } else if(quoted(exp)) {
    return text_of_quotation(exp);
  } else if(assignment(exp)) {
    //    return NULL;
    ret->data = "assignment\n";
  } else if(definition(exp)) {
    return eval_define(exp, env);
  } else if(lambda(exp)) {
    //    return NULL;
    ret->data = "lambda\n";
  } else if(if_exp(exp)) {
    //    return NULL;
    ret->data = "if\n";
  } else if(begin(exp)) {
    //    return NULL;
    ret->data = "begin\n";
  } else { // application
    return eval_appl(exp, env);
  }

  return ret;
}

LIST *eval_define(LIST *exp, LIST **env) {
  LIST *var = definition_variable(exp);
  LIST *val = eval(definition_value(exp), env);

  /* mutates the env */
  *env = define_variable((char*)var->data, val, *env);

  LIST *ret = malloc(sizeof(LIST));
  ret->type = ATOM;
  ret->data = "ok";
  ret->next = NULL;

  //  free(var);
  //  free(val);

  return ret;
}

LIST *eval_appl(LIST *exp, LIST **env) {
  /* eval the operator */
  LIST *op = eval(operator(exp), env);
  
  /* eval the operands */
  LIST *opands = operands(exp);
  LIST *ev_opands = NULL;
  for(LIST *n = opands; n != NULL; n = n->next) {
    if(ev_opands == NULL) {
      ev_opands = malloc(sizeof(LIST));
      ev_opands->next = NULL;
      ev_opands->type = ATOM;
      ev_opands->data = eval((LIST*)n->data, env);
      print_data(op);
      printf("\n");
    } else {
      LIST *d = eval((LIST*)n->data, env);
      append(ev_opands, d);
      print_data(op);
      printf("\n");
    }
  }

  /* apply the operator to the operands */
  return apply((PROCEDURE*)op->data, ev_opands);
}
