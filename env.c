#include <stdio.h>
#include "types.h"
#include "storage.h"
#include "env.h"
#include "apply.h"

object_t make_primitive(object_t (*f) (object_t)) {
  procedure_t p = procedure_new(NIL, NIL, NIL, f);
  return obj_new_procedure(p);
}

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
  object_t nil = obj_new_symbol("nil");

  object_t nilp = obj_new_symbol("nil?");

  env = define_variable(plus, make_primitive(&primitive_add), env);
  env = define_variable(asterisk, make_primitive(&primitive_multiply), env);
  env = define_variable(minus, make_primitive(&primitive_subtract), env);
  env = define_variable(fslash, make_primitive(&primitive_divide), env);
  env = define_variable(lessthan, make_primitive(&primitive_lessthan), env);
  env = define_variable(greaterthan, make_primitive(&primitive_greaterthan), env);
  env = define_variable(equals, make_primitive(&primitive_equals), env);
  env = define_variable(cons, make_primitive(&primitive_cons), env);
  env = define_variable(car, make_primitive(&primitive_car), env);
  env = define_variable(cdr, make_primitive(&primitive_cdr), env);
  env = define_variable(nilp, make_primitive(&primitive_nilp), env);
  env = define_variable(nil, NIL, env);

  return env;
}

/* lookup var in an alist or plist */
object_t assoc(object_t var, object_t list) {
  object_t l;
  for(l = list; !nilp(l); l = cdr(l)) {
    object_t a = car(l);
    object_t aa = car(a);
    if(symbolcmp(aa, var))
      return a;
  }
  return NIL;
}

/* search each frame in the environment for the var */
object_t lookup_variable(object_t var, object_t env) {
  object_t binding = NIL;
  while(!nilp(env)) {
    binding = assoc(var, car(env));
    if(!nilp(binding))
      break;
    env = cdr(env);
  }
  if(nilp(binding))
    return NIL;
  else
    return cdr(binding);
}

/* bind val to var and add it to this returning the new environment */
object_t define_variable(object_t var, object_t val, object_t env) {
  object_t binding; /* = lookup_variable(var, env);*/
  /*  if(!nilp(binding))
    printf("WARNING variable already defined: %s\n",
    obj_symbol_name(var));  */
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
  return define_variable(var, val, env);
}

/* match up vars and vals, and cons the resulting frame onto the env */
object_t extend_environment(object_t vars, object_t vals, object_t env) {
  object_t frame = NIL;
  while(!nilp(vars) && !nilp(vals)) {
    frame = cons(cons(car(vars), car(vals)), frame);
    vars = cdr(vars);
    vals = cdr(vals);
  }
  if(!nilp(vars) || !nilp(vals)) {
    printf("ERROR extend_environment: mismatching number of vars and vals.\n");
    return NIL;
  }
  env =  cons(frame, env);
  return env;
}
