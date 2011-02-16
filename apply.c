#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "types.h"
#include "apply.h"
#include "storage.h"
#include "env.h"
#include "obarray.h"

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
  char *s = symbol_name(obj_get_symbol(car(argl)));
  argl = cdr(argl);
  double result = atof(s);
  double arg;
  object_t a;
  while(!nilp(argl)) {
    s = symbol_name(obj_get_symbol(car(argl)));
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
  char *s = symbol_name(obj_get_symbol(car(argl)));
  argl = cdr(argl);
  double arg1 = atof(s);
  double arg2;
  object_t ret;
  while(!nilp(argl)) {
    s = symbol_name(obj_get_symbol(car(argl)));
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

object_t primitive_nilp(object_t argl) {
  if(nilp(car(argl))) {
    return obj_new_symbol("#t");
  } else {
    return obj_new_symbol("#f");
  }
}

extern object_t eval_sequence(object_t, object_t *);


object_t lambda_env(object_t exp) {
  return car(cdr(exp));
}

object_t lambda_params(object_t exp) {
  return car(cdr(cdr(exp)));
}

object_t lambda_body(object_t exp) {
  return cdr(cdr(cdr(exp)));
}

object_t apply(object_t p, object_t argl) {
  if(procp(p)) { /* primitive procedure */
    return (obj_get_proc(p))(argl);
  } else {
    object_t extended = extend_environment(lambda_params(p), argl, lambda_env(p));
    return eval_sequence(lambda_body(p), &extended); 
  }
}
