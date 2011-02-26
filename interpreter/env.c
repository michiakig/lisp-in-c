#include <stdio.h>
#include "types.h"
#include "storage.h"
#include "env.h"
#include "apply.h"

extern void load_file(char *filename, object_t *env);

object_t init_global() {
  object_t env = NIL;

  object_t primitives[] = {
    obj_new_symbol("+"),        obj_new_primitive(&primitive_add),
    obj_new_symbol("*"),        obj_new_primitive(&primitive_multiply),
    obj_new_symbol("-"),        obj_new_primitive(&primitive_subtract),
    obj_new_symbol("/"),        obj_new_primitive(&primitive_divide),
    obj_new_symbol("<"),        obj_new_primitive(&primitive_lessthan),
    obj_new_symbol(">"),        obj_new_primitive(&primitive_greaterthan),
    obj_new_symbol("="),        obj_new_primitive(&primitive_equals),
    obj_new_symbol("cons"),     obj_new_primitive(&primitive_cons),
    obj_new_symbol("car"),      obj_new_primitive(&primitive_car),
    obj_new_symbol("cdr"),      obj_new_primitive(&primitive_cdr),
    obj_new_symbol("null?"),     obj_new_primitive(&primitive_isnull),
    obj_new_symbol("eq?"),      obj_new_primitive(&primitive_eq),
    obj_new_symbol("set-cdr!"), obj_new_primitive(&primitive_set_cdr),
    obj_new_symbol("set-car!"), obj_new_primitive(&primitive_set_car),
    obj_new_symbol("symbol?"),  obj_new_primitive(&primitive_symbolp),
    obj_new_symbol("cons?"),    obj_new_primitive(&primitive_consp),
    obj_new_symbol("load"),     obj_new_primitive(&primitive_load),
    obj_new_symbol("print"),    obj_new_primitive(&primitive_print),
    obj_new_symbol("eval"),     obj_new_primitive(&primitive_eval),
    obj_new_symbol("apply"),    obj_new_primitive(&primitive_apply),
    obj_new_symbol("read"),     obj_new_primitive(&primitive_read),
    obj_new_symbol("read-file"),     obj_new_primitive(&primitive_read_file),
    obj_new_symbol("quit"),     obj_new_primitive(&primitive_quit),
    obj_new_symbol("error"),     obj_new_primitive(&primitive_error),
    obj_new_symbol("string?"),     obj_new_primitive(&primitive_stringp),
    obj_new_symbol("number?"),     obj_new_primitive(&primitive_numberp)
  };

  int i;
  for(i = 0; i < 52; i+=2)
    env = define_variable(primitives[i], primitives[i+1], env);

  return env;
}

/* lookup var in an alist or plist */
object_t assoc(object_t var, object_t list) {
  object_t l;
  for(l = list; !isnull(l); l = cdr(l)) {
    object_t a = car(l);
    object_t aa = car(a);
    if(obj_symbol_cmp(aa, var))
      return a;
  }
  return NIL;
}

object_t lookup_binding(object_t var, object_t env) {
  object_t binding = NIL;
  while(!isnull(env)) {
    binding = assoc(var, car(env));
    if(!isnull(binding))
      break;
    env = cdr(env);
  }
  return binding;
}

/* search each frame in the environment for the var */
object_t lookup_variable(object_t var, object_t env) {
  object_t binding = lookup_binding(var, env);
  if(isnull(binding))
    return NULL;
  else
    return cdr(binding);
}

/* bind val to var and add it to this returning the new environment */
object_t define_variable(object_t var, object_t val, object_t env) {
  object_t binding; /* = lookup_variable(var, env);*/
  binding = cons(var, val);

  object_t frame = NIL;
  if(!isnull(env))
    frame = car(env);
  frame = cons(binding, frame);

  if(!isnull(env))
    set_car(env, frame);
  else
    env = cons(frame, NIL);
  return env;
}

/* TODO: fix this after figuring out set-cdr! and set-car! */
object_t set_variable(object_t var, object_t val, object_t env) {
  object_t binding = lookup_binding(var, env);
  if(isnull(binding)) {
    printf("ERROR variable not defined: %s\n", symbol_name(obj_get_symbol(var)));
    return env;
  } else {
    set_cdr(binding, val);
    return env;
  }
}

/* match up vars and vals, and cons the resulting frame onto the env */
object_t extend_environment(object_t vars, object_t vals, object_t env) {
  object_t frame = NIL;
  while(!isnull(vars) && !isnull(vals)) {
    if(issymbol(vars)) {
      frame = cons(cons(vars, vals), frame);
      vars = NIL;
      vals = NIL;
    } else { /* normal args */
      frame = cons(cons(car(vars), car(vals)), frame);
      vars = cdr(vars);
      vals = cdr(vals);
    }
  }

  if(!isnull(vars) || !isnull(vals)) {
    printf("ERROR extend_environment: mismatching number of vars and vals.\n");
    return NIL;
  }
  env =  cons(frame, env);
  return env;
}
