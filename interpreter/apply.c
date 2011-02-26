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
#include "eval.h"
#include "runtime/reader.h"
#include "lib/str_utils.h"

struct proc_t {
  object_t (*fn) (object_t);
  object_t params;
  object_t body;
  object_t env;
};

int isprimitiveproc(proc_t p) {
  return p->fn != NULL;
}

int iscompoundproc(proc_t p) {
  return p->fn == NULL;
}

extern object_t global_env;
extern object_t load_file(char *, object_t *);
object_t read_file(char *);
extern void print_sequence(object_t);

int file_append(char *string, char *filename) {
  FILE *f = fopen(filename, "a");
  if(f==NULL)
    return 0;
  fprintf(f, "%s", string);
  fclose(f);
  return 1;
}

object_t primitive_file_append(object_t argl) {
  char *string = obj_get_string(car(argl));
  char *filename = obj_get_string(car(cdr(argl)));
  if(file_append(string, filename))
    return obj_new_symbol("#t");
  else
    return obj_new_symbol("#f");
}

object_t primitive_read_file(object_t argl) {
  object_t seq = read_file(obj_get_string(car(argl)));
  return seq;
}

object_t primitive_load(object_t argl) {
  object_t val = load_file(obj_get_string(car(argl)), &global_env);
  return obj_new_symbol("loaded.");
}

object_t primitive_consp(object_t argl) {
  if(iscons(car(argl)))
    return obj_new_symbol("#t");
  else
    return obj_new_symbol("#f");
}

object_t primitive_symbol2string(object_t argl) {
  return obj_new_string(symbol_name(obj_get_symbol(car(argl))));
}

object_t primitive_symbolp(object_t argl) {
  if(issymbol(car(argl)))
    return obj_new_symbol("#t");
  else
    return obj_new_symbol("#f");
}

object_t primitive_stringp(object_t argl) {
  if(isstring(car(argl)))
    return obj_new_symbol("#t");
  else
    return obj_new_symbol("#f");
}

object_t primitive_numberp(object_t argl) {
  char * s = symbol_name(obj_get_symbol(car(argl)));
  if(all_digits(s))
    return obj_new_symbol("#t");
  else
    return obj_new_symbol("#f");
}

object_t primitive_error(object_t argl) {
  primitive_print(argl);
  exit(0);
}

object_t primitive_quit(object_t argl) {
  exit(0);
}

object_t primitive_read(object_t argl) {
  object_t r = parse_sexp(read_sexp(stdin));
  if(r == NULL)
    return obj_new_symbol("_empty_");
  else
    return r;
}

object_t primitive_eval(object_t argl) {
  return eval(car(argl), &global_env);
}

object_t primitive_apply(object_t argl) {
  return apply(car(argl), car(cdr(argl)));
}

object_t primitive_print(object_t argl) {
  while(!isnull(argl)) {
    if(!isstring(car(argl)))
      print_object(car(argl));
    else
      printf("%s", obj_get_string(car(argl)));
    argl = cdr(argl);
  }
  return NULL;
}

object_t primitive_set_car(object_t argl) {
  set_car(car(argl), car(cdr(argl)));
  return obj_new_symbol("ok");
}

object_t primitive_set_cdr(object_t argl) {
  set_cdr(car(argl), car(cdr(argl)));
  return obj_new_symbol("ok");
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

object_t primitive_eq(object_t argl) {
  object_t a1 = car(argl);
  object_t a2 = car(cdr(argl));

  if(obj_symbol_cmp(a1, a2))
    return obj_new_symbol("#t");
  else
    return obj_new_symbol("#f");
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
  while(!isnull(argl)) {
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
  while(!isnull(argl)) {
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

object_t primitive_isnull(object_t argl) {
  if(isnull(car(argl))) {
    return obj_new_symbol("#t");
  } else {
    return obj_new_symbol("#f");
  }
}

extern object_t eval_sequence(object_t, object_t *);

object_t lambda_env(object_t p) {
  proc_t proc = obj_get_proc(p);
  return proc->env;
}

object_t lambda_params(object_t p) {
  proc_t proc = obj_get_proc(p);
  return proc->params;
}

object_t lambda_body(object_t p) {
  proc_t proc = obj_get_proc(p);
  return proc->body;  
}

object_t obj_new_compound(object_t params, object_t body, object_t env ) {
  proc_t proc = malloc(sizeof(*proc));
  proc->params = params;
  proc->body = body;
  proc->env = env;
  return obj_new_proc(proc);
}

object_t obj_new_primitive(object_t (*fn) (object_t)) {
  proc_t proc = malloc(sizeof(*proc));
  proc->fn = fn;
  return obj_new_proc(proc);
}

object_t apply(object_t p, object_t argl) {
  proc_t proc = obj_get_proc(p);
  if(isprimitiveproc(proc)) /* primitive procedure */
    return (proc->fn)(argl);
  else {
    object_t extended = extend_environment(lambda_params(p), argl, lambda_env(p));
    return eval_sequence(lambda_body(p), &extended);
  }
}
