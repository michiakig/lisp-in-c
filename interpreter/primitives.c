#include <stdlib.h> /* for malloc */
#include <stdio.h> /* for fgetc */
#include <math.h> /* for floor, log */
#include <string.h> /* strlen, strcat */

#include "primitives.h"
#include "types.h"
#include "storage.h"
#include "reader.h"
#include "eval.h"
#include "apply.h"

object_t read_file(char *);
extern void print_sequence(object_t);
extern object_t global_env;

object_t primitive_string_ref(object_t argl) {
  char *str = obj_get_string(car(argl));
  int i = obj_get_number(car(cdr(argl)));
  int len = strlen(str);
  if(i >= len)
    printf("ERROR string ref: index out of bounds\n");
  char s[2];
  s[0] = str[i];
  s[1] = '\0';
  return obj_new_string(s);
}

object_t primitive_string_equals(object_t argl) {
  int c = strcmp(obj_get_string(car(argl)), obj_get_string(car(cdr(argl))));
  return c==0 ? obj_new_symbol("#t") : obj_new_symbol("#f");
}

object_t primitive_string_length(object_t argl) {
  int len = strlen(obj_get_string(car(argl)));
  return obj_new_number(len);
}

object_t primitive_read_char(object_t argl) {
  int c = fgetc(stdin);
  char s[2];
  s[0] = (char)c;
  s[1] = '\0';
  object_t ret = obj_new_string(s);
  return ret;
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
