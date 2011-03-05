#include <stdio.h>

#include "types.h"
#include "storage.h"
#include "env.h"

object_t global_env;

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

  if(isnull(vals) && issymbol(vars))
    frame = cons(cons(vars, vals), frame);
  else if(!isnull(vars) || !isnull(vals)) {
    printf("ERROR extend_environment: mismatching number of vars and vals.");
    return NIL;
  }
  env =  cons(frame, env);
  return env;
}
