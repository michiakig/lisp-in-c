#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "eval.h"
#include "apply.h"

#include "storage.h"
#include "env.h"
#include "obarray.h"
#include "types.h"
#include "reader.h"

#include "lib/str_utils.h"

extern object_t global_env;
object_t read_file(char *);
extern void print_sequence(object_t);


object_t init_global() {
  object_t env = NIL;
  object_t primitives[] = {
    obj_new_symbol("+"),         obj_new_primitive(&primitive_add),
    obj_new_symbol("*"),         obj_new_primitive(&primitive_multiply),
    obj_new_symbol("-"),         obj_new_primitive(&primitive_subtract),
    obj_new_symbol("/"),         obj_new_primitive(&primitive_divide),
    obj_new_symbol("<"),         obj_new_primitive(&primitive_lessthan),
    obj_new_symbol(">"),         obj_new_primitive(&primitive_greaterthan),
    obj_new_symbol("="),         obj_new_primitive(&primitive_equals),
    obj_new_symbol("cons"),      obj_new_primitive(&primitive_cons),
    obj_new_symbol("car"),       obj_new_primitive(&primitive_car),
    obj_new_symbol("cdr"),       obj_new_primitive(&primitive_cdr),
    obj_new_symbol("null?"),     obj_new_primitive(&primitive_isnull),
    obj_new_symbol("eq?"),       obj_new_primitive(&primitive_eq),
    obj_new_symbol("set-cdr!"),  obj_new_primitive(&primitive_set_cdr),
    obj_new_symbol("set-car!"),  obj_new_primitive(&primitive_set_car),
    obj_new_symbol("symbol?"),   obj_new_primitive(&primitive_symbolp),
    obj_new_symbol("cons?"),     obj_new_primitive(&primitive_consp),
    obj_new_symbol("load"),      obj_new_primitive(&primitive_load),
    obj_new_symbol("print"),     obj_new_primitive(&primitive_print),
    obj_new_symbol("eval"),      obj_new_primitive(&primitive_eval),
    obj_new_symbol("apply"),     obj_new_primitive(&primitive_apply),
    obj_new_symbol("read"),      obj_new_primitive(&primitive_read),
    obj_new_symbol("read-file"), obj_new_primitive(&primitive_read_file),
    obj_new_symbol("quit"),      obj_new_primitive(&primitive_quit),
    obj_new_symbol("error"),     obj_new_primitive(&primitive_error),
    obj_new_symbol("string?"),   obj_new_primitive(&primitive_stringp),
    obj_new_symbol("number?"),   obj_new_primitive(&primitive_numberp),
    obj_new_symbol("file-append"),    obj_new_primitive(&primitive_file_append),
    obj_new_symbol("symbol->string"), obj_new_primitive(&primitive_symbol2string),
    obj_new_symbol("string->symbol"), obj_new_primitive(&primitive_string2symbol),
    obj_new_symbol("string-append"), obj_new_primitive(&primitive_string_append),
    obj_new_symbol("number->string"), obj_new_primitive(&primitive_number2string)
  };

  int i;
  for(i = 0; i < 62; i+=2)
    env = define_variable(primitives[i], primitives[i+1], env);

  return env;
}

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

object_t load_file(char *filename, object_t *env) {
  object_t exp = read_file(filename);
  return eval(exp, env);
}

object_t primitive_read_file(object_t argl) {
  object_t seq = read_file(obj_get_string(car(argl)));
  return seq;
}

object_t primitive_load(object_t argl) {
  load_file(obj_get_string(car(argl)), &global_env);
  return obj_new_symbol("loaded.");
}

object_t primitive_consp(object_t argl) {
  if(iscons(car(argl)))
    return obj_new_symbol("#t");
  else
    return obj_new_symbol("#f");
}

object_t primitive_number2string(object_t argl) {
  int i = obj_get_number(car(argl));
  char *s;
  if(i == 0)
    s = malloc(sizeof(*s) * 1);
  else
    s = malloc(sizeof(*s) * (int)floor(log10(i)) + 1);

  sprintf(s, "%d", i);
  object_t ret =  obj_new_string(s);
  free(s);
  return ret;
}

object_t primitive_string_append(object_t argl) {
  char *s1 = obj_get_string(car(argl));
  char *s2 = obj_get_string(car(cdr(argl)));
  int len = strlen(s1) + strlen(s2);
  char *s = malloc(sizeof(*s) * (len)+1);
  s[0] = '\0';
  strcat(s, s1);
  strcat(s, s2);
  object_t ret = obj_new_string(s);
  free(s);
  return ret;
}

object_t primitive_symbol2string(object_t argl) {
  return obj_new_string(symbol_name(obj_get_symbol(car(argl))));
}

object_t primitive_string2symbol(object_t argl) {
  return obj_new_symbol(obj_get_string(car(argl)));
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
  if(isnum(car(argl)))
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

  int result;

  if(issymbol(a1) && issymbol(a2))
    result = obj_symbol_cmp(a1, a2);
  else if(isnum(a1) && isnum(a2))
    result = (obj_get_number(a1) == obj_get_number(a2));
  else
    result = 0;

  if(result)
    return obj_new_symbol("#t");
  else
    return obj_new_symbol("#f");
}
  

/* Wrappers around built-in C arithmetic operators */
int add(int a1, int a2) { return a1 + a2; }
int multiply(int a1, int a2) { return a1 * a2; }
int subtract(int a1, int a2) { return a1 - a2; }
int divide(int a1, int a2) { return a1 / a2; }

int lessthan(int a1, int a2) { return a1 < a2; }
int greaterthan(int a1, int a2) { return a1 > a2; }
int equals(int a1, int a2) { return a1 == a2; }

/* Fold (left) across an argument list, applying one of the num wrappers above */
object_t num_primitive(object_t argl, int (*f) (int, int)) {
  int result = obj_get_number(car(argl));
  argl = cdr(argl);
  int arg;
  while(!isnull(argl)) {
    arg = obj_get_number(car(argl));
    result = f(result, arg);
    argl = cdr(argl);
  }
  object_t ret = obj_new_number(result);
  return ret;
}

object_t cmp_primitive(object_t argl, int (*f) (int, int)) {
  int arg1 = obj_get_number(car(argl));
  argl = cdr(argl);
  int arg2;
  object_t ret;
  while(!isnull(argl)) {
    arg2 = obj_get_number(car(argl));
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
