#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "types.h"
#include "list.h"

#include "eval.h"
#include "apply.h"
#include "env.h"
#include "print.h"

extern struct nlist *hashtable[];

void init_proc(procedure *p, list *params, list *body,
               list *env, list* (*fn) (list *argl)) {
  p->params = params;
  p->body = body;
  p->env = env;
  p->fn = fn;
}

/* Wrappers around built-in C arithmetic operators  */
double add(double a1, double a2) { return a1 + a2; }
double multiply(double a1, double a2) { return a1 * a2; }
double subtract(double a1, double a2) { return a1 - a2; }
double divide(double a1, double a2) { return a1 / a2; }

int lt(double a1, double a2) { return a1 < a2; }
int gt(double a1, double a2) { return a1 > a2; }
int eq(double a1, double a2) { return a1 == a2; }

/* Fold (left) across an argument list, applying one of the numeric wrappers above */
list *numeric_primitive(list *argl, double (*f) (double, double)) {
  char *s = argl->data.listData->data.symbolData->name;
  double result = atof(s);
  double arg;
  for(list *a = argl->next; a != NULL; a = a->next) {
    s = a->data.listData->data.symbolData->name;
    arg = atof(s);
    result = f(result, arg);
  }
  s = malloc(sizeof(char)*100); /* TODO: calculate the num digits */
  sprintf(s, "%f", result);
  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Symbol, intern(s, hashtable));
  return ret;
}

list *compare_primitive(list *argl, int (*f) (double, double)) {
  char *s = argl->data.listData->data.symbolData->name;
  double arg1 = atof(s);
  double arg2;
  list *ret = malloc(sizeof(list));
  for(list *a = argl->next; a != NULL; a = a->next) {
    s = a->data.listData->data.symbolData->name;
    arg2 = atof(s);
    if(!f(arg1, arg2)) {
      init_list(ret, NULL, Symbol, intern("#f", hashtable));
      return ret;
    }
    arg1 = arg2;
  }
    init_list(ret, NULL, Symbol, intern("#t", hashtable));
  return ret;
}

/* Call the foldl procedure above with the appropriate wrapper */
list *primitive_add(list *argl) { return numeric_primitive(argl, &add); }
list *primitive_multiply(list *argl) { return numeric_primitive(argl, &multiply); }
list *primitive_subtract(list *argl) { return numeric_primitive(argl, &subtract); }
list *primitive_divide(list *argl) { return numeric_primitive(argl, &divide); }

list *primitive_lt(list *argl) { return compare_primitive(argl, &lt); }
list *primitive_gt(list *argl) { return compare_primitive(argl, &gt); }
list *primitive_eq(list *argl) { return compare_primitive(argl, &eq); }

list *apply(procedure *p, list *argl) {
  if(p->fn != NULL) { // primitive procedure
    return (*(p->fn))(argl);
  } else {
    list *extended_env = extend_environment(p->params, argl, p->env);
    return eval_sequence(p->body, &extended_env);
  }
}
