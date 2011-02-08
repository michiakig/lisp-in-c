#include <string.h> /* for strcmp */
#include <stdio.h> /* for printf */
#include <stdlib.h> /* for malloc */

#include "types.h"
#include "list.h"

#include "env.h"
#include "eval.h"
#include "apply.h"
#include "print.h"

list *make_primitive(list* (*f) (list*)) {
  procedure *p = malloc(sizeof(procedure));
  init_proc(p, NULL, NULL, NULL, f);
  list *l = malloc(sizeof(list));
  init_list(l, NULL, Procedure, p);
  return l;
}

list *init_global(struct nlist *hashtable[]) {
  list *env = NULL;
  
  symbol *plus = intern("+", hashtable);
  symbol *asterisk = intern("*", hashtable);
  symbol *minus = intern("-", hashtable);
  symbol *fslash = intern("/", hashtable);
  symbol *lt = intern("<", hashtable);
  symbol *gt = intern(">", hashtable);
  symbol *equals = intern("=", hashtable);

  env = define_variable(plus, make_primitive(&primitive_add), env);
  env = define_variable(asterisk, make_primitive(&primitive_multiply), env);
  env = define_variable(minus, make_primitive(&primitive_subtract), env);
  env = define_variable(fslash, make_primitive(&primitive_divide), env);

  env = define_variable(lt, make_primitive(&primitive_lt), env);
  env = define_variable(gt, make_primitive(&primitive_gt), env);
  env = define_variable(equals, make_primitive(&primitive_eq), env);

  return env;
}

/*
 * An environment is a list of frames. A frame is a list of binds
 * A bind is a (name, value) binding pair defined as a struct, name is a char*
 * and value is a list*
 */

binding *lookup_variable_binding(symbol *var, list *env) {
  if(env == NULL) {
    return NULL;
  } else {
    binding *bind;
    for(list *frame = env->data.listData; frame != NULL; frame = frame->next) {
      bind = frame->data.bindData;
      if(var == bind->name) // just compare symbol pointers
        return bind;
    }
    return lookup_variable_binding(var, env->next);
  }
}

list *lookup_variable_value(symbol *var, list *env) {
  binding *bind = lookup_variable_binding(var, env);
  if(bind == NULL) {
    printf("ERROR undefined variable: %s\n", var->name);
    return NULL;
  } else {
    return bind->value;
  }
}

list *define_variable(symbol *var, list *value, list *env) {
  binding *bind = lookup_variable_binding(var, env);
  if(bind != NULL) {
    printf("WARNING variable already defined: %s\n", var->name);
  }

  /* allocate a new binding*/
  bind = malloc(sizeof(binding));
  bind->name = var;
  bind->value = value;

  /* allocate a new node to be added to the frame */
  list *node = malloc(sizeof(list));
  init_list(node, NULL, Binding, bind);

  /* get the frame, allocating a new env if needed */
  list *frame = NULL;
  if(env == NULL) {
    env = malloc(sizeof(list));
    init_list(env, NULL, List, NULL);
  } else {
    frame = env->data.listData;
  }

  /* if the env was NULL or the env was empty, set frame to the new node */
  if(frame == NULL)
    frame = node;
  else /* else cons the node onto the frame */
    frame = cons(node, frame);

  env->data.listData = frame;
  return env;
}

list *set_variable(symbol *var, list *value, list *env) {
  binding *bind = lookup_variable_binding(var, env);
  if(bind == NULL) {
    printf("ERROR undefined variable: %s\n", var->name);
    return NULL;
  }

  bind->value = value; // TODO: fix this memory leak!
  return env;
}

list *extend_environment(list *vars, list *vals, list *env) {
  list *frame = NULL;
  list *r, *l;
  for(r = vars, l = vals; r != NULL && l != NULL; r = r->next, l = l->next) {
    binding *bind = malloc(sizeof(bind));
    bind->name = getSymbol(r);
    bind->value = getList(l);
    list *frame_node = malloc(sizeof(list));
    init_list(frame_node, NULL, Binding, bind);
    frame = cons(frame_node, frame);
  }

  if(r != NULL || l != NULL) {
    printf("ERROR extend_environment: mismatching number of vars and vals.\n");
    return NULL;
  }

  list *env_node = malloc(sizeof(list));
  init_list(env_node, NULL, List, frame);

  return cons(env_node, env);
}
