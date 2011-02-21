#include <stdio.h>
#include "types.h"
#include "storage.h"
#include "env.h"
#include "apply.h"

extern void load_file(char *filename, object_t *env);

object_t init_global() {
  object_t env = NIL;

  object_t plus = obj_new_symbol("+");
  object_t asterisk = obj_new_symbol("*");
  object_t minus = obj_new_symbol("-");
  object_t fslash = obj_new_symbol("/");
  object_t lessthan = obj_new_symbol("<");
  object_t greaterthan = obj_new_symbol(">");
  object_t equals = obj_new_symbol("=");

  object_t cons = obj_new_symbol("cons");
  object_t car = obj_new_symbol("car");
  object_t cdr = obj_new_symbol("cdr");
  object_t nilp = obj_new_symbol("nil?");
  object_t eq = obj_new_symbol("eq?");
  object_t load = obj_new_symbol("load");
  object_t scdr = obj_new_symbol("set-cdr!");
  object_t scar = obj_new_symbol("set-car!");

  env = define_variable(plus, obj_new_proc(&primitive_add), env);
  env = define_variable(asterisk, obj_new_proc(&primitive_multiply), env);
  env = define_variable(minus, obj_new_proc(&primitive_subtract), env);
  env = define_variable(fslash, obj_new_proc(&primitive_divide), env);
  env = define_variable(lessthan, obj_new_proc(&primitive_lessthan), env);
  env = define_variable(greaterthan, obj_new_proc(&primitive_greaterthan), env);
  env = define_variable(equals, obj_new_proc(&primitive_equals), env);

  env = define_variable(cons, obj_new_proc(&primitive_cons), env);
  env = define_variable(car, obj_new_proc(&primitive_car), env);
  env = define_variable(cdr, obj_new_proc(&primitive_cdr), env);
  env = define_variable(nilp, obj_new_proc(&primitive_nilp), env);
  env = define_variable(eq, obj_new_proc(&primitive_eq), env);
  env = define_variable(load, obj_new_proc(&primitive_load), env);
  env = define_variable(scdr, obj_new_proc(&primitive_set_cdr), env);
  env = define_variable(scar, obj_new_proc(&primitive_set_car), env);
  
  return env;
}

/* lookup var in an alist or plist */
object_t assoc(object_t var, object_t list) {
  object_t l;
  for(l = list; !nilp(l); l = cdr(l)) {
    object_t a = car(l);
    object_t aa = car(a);
    if(obj_symbol_cmp(aa, var))
      return a;
  }
  return NIL;
}

object_t lookup_binding(object_t var, object_t env) {
  object_t binding = NIL;
  while(!nilp(env)) {
    binding = assoc(var, car(env));
    if(!nilp(binding))
      break;
    env = cdr(env);
  }
  return binding;
}

/* search each frame in the environment for the var */
object_t lookup_variable(object_t var, object_t env) {
  object_t binding = lookup_binding(var, env);
  if(nilp(binding))
    return NULL;
  else
    return cdr(binding);
}

/* bind val to var and add it to this returning the new environment */
object_t define_variable(object_t var, object_t val, object_t env) {
  object_t binding; /* = lookup_variable(var, env);*/
  binding = cons(var, val);

  object_t frame = NIL;
  if(!nilp(env))
    frame = car(env);
  frame = cons(binding, frame);

  if(!nilp(env))
    set_car(env, frame);
  else
    env = cons(frame, NIL);
  return env;
}

/* TODO: fix this after figuring out set-cdr! and set-car! */
object_t set_variable(object_t var, object_t val, object_t env) {
  object_t binding = lookup_binding(var, env);
  if(nilp(binding)) {
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
  while(!nilp(vars) && !nilp(vals)) {
    if(obj_symbol_cmp(car(vars), obj_new_symbol("."))) {
      /* handling of dotted tail args */
      frame = cons(cons(car(cdr(vars)), vals), frame);
      vars = cdr(cdr(vars));
      vals = NIL;
    } else { /* normal args */
      frame = cons(cons(car(vars), car(vals)), frame);
      vars = cdr(vars);
      vals = cdr(vals);
    }
  }

  if(!nilp(vars) || !nilp(vals)) {
    printf("ERROR extend_environment: mismatching number of vars and vals.\n");
    return NIL;
  }
  env =  cons(frame, env);
  return env;
}
