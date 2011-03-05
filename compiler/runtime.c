/* A simple runtime for the compiler */
#include <stdio.h>
#include <stdlib.h>
#include "runtime.h"
#include "../interpreter/storage.h"
#include "../interpreter/env.h"
#include "../interpreter/apply.h"
#include "../interpreter/eval.h"

int pc;
int flag;
object_t reg[7] = {0};
object_t stack[STACKSIZE] = {0};
object_t *sp = stack;

extern object_t entry(void);

object_t lookup_variable_value(object_t variable, object_t environment) {
  return lookup_variable(variable, environment);
}

int primitive_procedurep(object_t procedure) {
  return isproc(procedure); // non-primitive procedures are compiled
                            // and are Conses
}

object_t apply_primitive_procedure(object_t procedure, object_t arglist) {
  return apply(procedure, arglist);
}

int falsep(object_t obj) {
  return truthy(obj) ? 0 : 1;
}

object_t list(object_t obj) {
  return cons(obj, NIL);
}

// See footnote 38 in chapter 5:
// machine operations to implement a data structure for representing
// compiled procedures

object_t make_compiled_procedure(object_t entry, object_t environment) {
  return cons(obj_new_symbol("compiled-procedure"),
              cons(entry,
                   cons(environment, NIL)));
}

object_t compiled_procedure_entry(object_t procedure) {
  return car(cdr(procedure));
}

object_t compiled_procedure_env(object_t procedure) {
  return car(cdr(cdr(procedure)));
}

void save(object_t x) {
  *sp = x; sp++;
}

object_t restore() {
  sp--; return *sp;
}

int obj2label(object_t obj) {
  return obj_get_number(obj);
}

object_t label2obj(int label) {
  return obj_new_number(label);
}

int main(int argc, char **argv) {
  printf("output:");
  print_object(entry());
  printf("\n");
}
