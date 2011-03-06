#include <stdio.h>

#include "../interpreter/types.h"
#include "storage.h"
#include "env.h"
#include "runtime.h"

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

object_t lookup_binding(object_t var, object_t environ) {
  object_t binding = NIL;
  while(!isnull(environ)) {
    binding = assoc(var, car(environ));
    if(!isnull(binding))
      break;
    environ = cdr(environ);
  }
  return binding;
}

/* search each frame in the environment for the var */
object_t lookup_variable(object_t var, object_t environ) {
  object_t binding = lookup_binding(var, environ);
  if(isnull(binding))
    return NULL;
  else
    return cdr(binding);
}

object_t set_variable(object_t var, object_t value, object_t environ) {
  object_t binding = lookup_binding(var, environ);
  if(isnull(binding)) {
    printf("ERROR variable not defined: %s\n", symbol_name(obj_get_symbol(var)));
    return environ;
  } else {
     set_cdr(binding, value);
    return environ;
  }
}

object_t define_variable(object_t var, object_t value, object_t environ) {
  save(reg[gen1]);
  save(reg[gen2]);
  save(reg[gen3]);
  save(reg[unev]);

  reg[gen1] = var;
  reg[gen2] = value;
  reg[gen3] = environ;
  
  reg[gen1] = cons(reg[gen1], reg[gen2]);

  reg[unev] = NIL;
  if(!isnull(reg[gen3]))
    reg[unev] = car(reg[gen3]);
  
  reg[unev] = cons(reg[gen1], reg[unev]);

  if(!isnull(reg[gen3]))
    set_car(reg[gen3], reg[unev]);
  else
    reg[gen3] = cons(reg[unev], NIL);

  object_t ret = reg[gen3];
  reg[unev]=restore();
  reg[gen3]=restore();
  reg[gen2]=restore();
  reg[gen1]=restore();
  return ret;
}

object_t extend_environment(object_t vars, object_t vals, object_t environ) {
  save(reg[gen1]); /* save general registers */
  save(reg[gen2]);
  save(reg[gen3]);
  save(reg[unev]); /* save unev, will be used below */
  save(reg[env]);
  
  reg[env] = environ;
  reg[unev]=NIL; /* the frame */
  reg[gen2]=vars;
  reg[gen3]=vals;

  while(!isnull(reg[gen2]) && !isnull(reg[gen3])) {
    /* cons car(vars) to car(vals) to make a binding */
    reg[gen1] = cons(car(reg[gen2]), car(reg[gen3]));
    /* cons binding onto the frame */
    reg[unev] = cons(reg[gen1], reg[unev]);
    /* cdr down the vars and vals */
    reg[gen2] = cdr(reg[gen2]);
    reg[gen3] = cdr(reg[gen3]);
  }
  if(!isnull(reg[gen2]) || !isnull(reg[gen3])) {
    printf("ERROR extend: mismatching number of vars and vals.");
    return NIL;
  }
  environ = cons(reg[unev], reg[env]);
  reg[env]=restore();
  reg[unev]=restore();
  reg[gen3]=restore();
  reg[gen2]=restore();
  reg[gen1]=restore();

  return environ;
}
