#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "types.h"
#include "apply.h"
#include "storage.h"
#include "env.h"

struct procedure_t {
  object_t params;
  object_t body;
  object_t env;
  object_t (*fn) (object_t argl);
};

procedure_t procedure_new(object_t params, object_t body, object_t env,
                          object_t (*fn) (object_t)) {

  procedure_t p = malloc(sizeof(*p));
  p->params = params;
  p->body = body;
  p->env = env;
  p->fn = fn;
  return p;
}

object_t primitive_cons(object_t argl) {
  object_t a = car(argl);
  object_t d = car(cdr(argl));
  return cons(a, d);
}

object_t primitive_car(object_t argl) {
  object_t o = car(argl);
  return car(o);
}

object_t primitive_cdr(object_t argl) {
  object_t o = car(argl);
  return cdr(o);
}


/* Wrappers around built-in C arithmetic operators */
double add(double a1, double a2) { return a1 + a2; }
double multiply(double a1, double a2) { return a1 * a2; }
double subtract(double a1, double a2) { return a1 - a2; }
double divide(double a1, double a2) { return a1 / a2; }

int lessthan(double a1, double a2) { return a1 < a2; }
int greaterthan(double a1, double a2) { return a1 > a2; }
int equals(double a1, double a2) { return a1 == a2; }

/* Fold (left) across an argument list, applying one of the num wrappers above */
object_t num_primitive(object_t argl, double (*f) (double, double)) {
  char *s = obj_symbol_name(car(argl));
  argl = cdr(argl);
  double result = atof(s);
  double arg;
  object_t a;
  while(!nilp(argl)) {
    s = obj_symbol_name(car(argl));
    arg = atof(s);
    result = f(result, arg);
    argl = cdr(argl);
  }
  s = malloc(sizeof(char)*100); /* TODO: calculate the num digits */
  sprintf(s, "%f", result);
  object_t ret = obj_new_symbol(s);
  free(s);
  return ret;
}

object_t cmp_primitive(object_t argl, int (*f) (double, double)) {
  char *s = obj_symbol_name(car(argl));
  argl = cdr(argl);
  double arg1 = atof(s);
  double arg2;
  object_t ret;
  while(!nilp(argl)) {
    s = obj_symbol_name(car(argl));
    arg2 = atof(s);
    if(!f(arg1, arg2)) {
      ret = obj_new_symbol("#f");
      return ret;
    }
    arg1 = arg2;
    argl = cdr(argl);
  }
  ret = obj_new_symbol("#t");
  return ret;
}

/* Call the foldl procedure above with the appropriate wrapper */
object_t primitive_add(object_t argl) {
  return num_primitive(argl, &add);
}
object_t primitive_multiply(object_t argl) {
  return num_primitive(argl, &multiply);
}
object_t primitive_subtract(object_t argl) {
  return num_primitive(argl, &subtract);
}
object_t primitive_divide(object_t argl) {
  return num_primitive(argl, &divide);
}
object_t primitive_lessthan(object_t argl) {
  return cmp_primitive(argl, &lessthan);
}
object_t primitive_greaterthan(object_t argl) {
  return cmp_primitive(argl, &greaterthan);
}
object_t primitive_equals(object_t argl) {
  return cmp_primitive(argl, &equals);
}

extern object_t eval_sequence(object_t, object_t *);

object_t apply(procedure_t p, object_t argl) {
  if(p->fn != NULL) { /* primitive procedure */
    return (*(p->fn))(argl);
  } else {
    object_t extended = extend_environment(p->params, argl, p->env);
    return eval_sequence(p->body, &extended); 
  }
}
