#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "apply.h"
#include "llist.h"
#include "str_utils.h"
#include "read.h"
#include "syntax.h"
#include "env.h"

LLIST *stack;
LLIST *expr;
LLIST *global_env;

DATA cont;
DATA proc;
LLIST *argl;
LLIST *unev;

char *in_prompt = ";;; C-eval input:";
char *val_prompt = ";;; C-eval value:";

LLIST *init_env();

LLIST *init_env() {
  LLIST *env = malloc(sizeof(LLIST));
  env->data = NULL;
  env->primitive = 0;
  env->next = NULL;

  LLIST *frame = malloc(sizeof(LLIST));

  PROCEDURE *sum_proc = malloc(sizeof(PROCEDURE));
  sum_proc->env = NULL;
  sum_proc->params = NULL;
  sum_proc->body = NULL;
  sum_proc->fun = primitive_add;
  
  DATA *d = malloc(sizeof(DATA));
  d->type = PROC;
  d->data = sum_proc;

  BIND *b = malloc(sizeof(BIND));
  b->name = "plus";
  b->value = d;

  frame->data = b;
  frame->next = NULL;

  env->data = frame;

  return env;
}

int main(int argc, char **argv) {
  /* initialize global environment */
  global_env = init_env();

  printf(";;; type \"quit\" to quit\n");
  LLIST *input = NULL;
  while(1) {
    printf("%s ", in_prompt);
    input = read_sexp(stdin);
    if(input != NULL) {
      if(input->data != NULL) {
        if(strcmp((char*)input->data, "quit\n") == 0) {
          exit(0);
        }


      }

      DATA *exp = parse_sexp(input);

      //free_recur(input);
      printf("%s\n", val_prompt);
      DATA *val = eval(exp, &global_env);
      if(val != NULL) {
        print_data(val);
        printf("\n");
      } else {
        printf("error.\n");
      }

      //      free(exp->data);
      //      free(exp);
    }
  }
}

void print_data(DATA *d) {
  if(d != NULL) {
    if(d->data != NULL) {
      if(d->type == ATOM) {
        printf("%s", (char*)d->data);
      } else if(d->type == PROC) {
        printf("<proc ...>");
      } else {
        list_print_sexp((LLIST*)d->data);
      }
    }
  }
}

DATA *eval(DATA *expr, LLIST **env) {
  if(self_evaluating(expr)) {
    return expr;
  } else if(variable(expr)) {
    return lookup_variable_value((char*)expr->data, *env);
  } else if(quoted(expr)) {
    return text_of_quotation(expr);
  } else if(assignment(expr)) {
    return NULL;
  } else if(definition(expr)) {
    return eval_define(expr, env);
  } else if(lambda(expr)) {
    return NULL;
  } else if(if_expr(expr)) {
    return NULL;
  } else if(begin(expr)) {
    return NULL;
  } else { // application
    return eval_appl(expr, env);
  }
}

DATA *eval_define(DATA *expr, LLIST **env) {
  DATA *var = definition_variable(expr);
  DATA *val = eval(definition_value(expr), env);

  /* mutates the env */
  *env = define_variable((char*)var->data, val, *env);

  DATA *d = malloc(sizeof(DATA));
  d->type = ATOM;
  d->data = "ok";

  //  free(var);
  //  free(val);

  return d;
}

DATA *eval_appl(DATA *expr, LLIST **env) {

  /* eval the operator */
  DATA *op = eval(operator(expr), env);
  
  /* eval the operands */
  LLIST *opands = operands(expr);
  LLIST *ev_opands = NULL;
  for(LLIST *n = opands; n != NULL; n = n->next) {
    if(ev_opands == NULL) {
      ev_opands = malloc(sizeof(LLIST));
      ev_opands->next = NULL;
      ev_opands->primitive = 1;
      ev_opands->data = eval((DATA*)n->data, env);
      print_data(op);
      printf("\n");
    } else {
      DATA *d = eval((DATA*)n->data, env);
      list_append(ev_opands, d, 1);
      print_data(op);
      printf("\n");
    }
  }

  /* apply the operator to the operands */
  return apply((PROCEDURE*)op->data, ev_opands);
}
