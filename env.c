#include <stdio.h>
#include "storage.h"
#include "env.h"

/*
list *make_primitive(list* (*f) (list*)) {
  procedure *p = malloc(sizeof(procedure));
  init_proc(p, NULL, NULL, NULL, f);
  list *l = malloc(sizeof(list));
  init_list(l, NULL, Procedure, p);
  return l;
}
*/
/*
list *init_global(struct nlist *hashtable[]) {
  list *env = NULL;
  
  symbol *plus = intern("+", hashtable);
  symbol *asterisk = intern("*", hashtable);
  symbol *minus = intern("-", hashtable);
  symbol *fslash = intern("/", hashtable);
  symbol *lt = intern("<", hashtable);
  symbol *gt = intern(">", hashtable);
  symbol *equals = intern("=", hashtable);

  symbol *cons = intern("cons", hashtable);
  symbol *car = intern("car", hashtable);
  symbol *cdr = intern("cdr", hashtable);

  env = define_variable(plus, make_primitive(&primitive_add), env);
  env = define_variable(asterisk, make_primitive(&primitive_multiply), env);
  env = define_variable(minus, make_primitive(&primitive_subtract), env);
  env = define_variable(fslash, make_primitive(&primitive_divide), env);

  env = define_variable(lt, make_primitive(&primitive_lt), env);
  env = define_variable(gt, make_primitive(&primitive_gt), env);
  env = define_variable(equals, make_primitive(&primitive_eq), env);

  env = define_variable(cons, make_primitive(&primitive_cons), env);
  env = define_variable(car, make_primitive(&primitive_car), env);
  env = define_variable(cdr, make_primitive(&primitive_cdr), env);

  return env;
}
*/

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
  object_t frame;
  for(frame = (nilp(env) ? NIL : car(env)); !nilp(frame); frame = cdr(frame)) {
    binding = assoc(var, frame);
    if(!nilp(binding))
      break;
  }
  if(nilp(binding))
    return NIL;
  else
    return cdr(binding);
}

/* bind val to var and add it to this returning the new environment */
object_t define_variable(object_t var, object_t val, object_t env) {
  object_t binding = lookup_variable(var, env);
  /*  if(!nilp(binding))
    printf("WARNING variable already defined: %s\n",
    obj_symbol_name(var));  */
  binding = cons(var, val);
  object_t frame = NIL;
  if(!nilp(env))
    frame = car(env);
  frame = cons(binding, frame);
  if(!nilp(env))
    env = cons(frame, cdr(env));
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
  object_t var = vars;
  object_t val = vals;
  object_t frame = NIL;
  while(!nilp(var) && nilp(val)) {
    frame = cons(cons(var, val), frame);
    var = cdr(var);
    val = cdr(val);
  }
  if(!nilp(var) || !nilp(val)) {
    printf("ERROR extend_environment: mismatching number of vars and vals.\n");
    return NIL;
  }
  return cons(frame, env);
}

