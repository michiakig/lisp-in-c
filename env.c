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
  proc *p = malloc(sizeof(proc));
  init_proc(p, NULL, NULL, NULL, f);
  list *l = malloc(sizeof(list));
  init_list(l, NULL, Proc, p);
  return l;
}

list *init_global() {
  list *env = NULL;
  env = define_variable("+", make_primitive(&primitive_add), env);
  env = define_variable("*", make_primitive(&primitive_multiply), env);
  env = define_variable("-", make_primitive(&primitive_subtract), env);
  env = define_variable("/", make_primitive(&primitive_divide), env);

  env = define_variable("<", make_primitive(&primitive_lt), env);
  env = define_variable(">", make_primitive(&primitive_gt), env);
  env = define_variable("=", make_primitive(&primitive_eq), env);

  return env;
}

/*
 * An environment is a list of frames. A frame is a list of binds
 * A bind is a (name, value) binding pair defined as a struct, name is a char*
 * and value is a list*
 */

bind *lookup_variable_binding(char *variable, list *env) {
  if(env == NULL) {
    return NULL;
  } else {
    //    print_env(env);
    //    printf("\n");

    bind *binding;
    for(list *frame = env->kindData.listData; frame != NULL; frame = frame->next) {
      binding = frame->kindData.bindData;
      if(strcmp(variable, binding->name) == 0)
        return binding;
    }
    return lookup_variable_binding(variable, env->next);
  }
}

list *lookup_variable_value(char *variable, list *env) {
  bind *binding = lookup_variable_binding(variable, env);
  if(binding == NULL) {
    printf("ERROR undefined variable: %s\n", variable);
    return NULL;
  } else {
    return binding->value;
  }
}

list *define_variable(char *var, list *value, list *env) {
  bind *binding = lookup_variable_binding(var, env);
  if(binding != NULL) {
    printf("WARNING variable already defined: %s\n", var);
  }

  /* allocate a new binding*/
  binding = malloc(sizeof(bind));
  binding->name = var;
  binding->value = value;

  /* allocate a new node to be added to the frame */
  list *node = malloc(sizeof(list));
  init_list(node, NULL, Bind, binding);

  /* get the frame, allocating a new env if needed */
  list *frame = NULL;
  if(env == NULL) {
    env = malloc(sizeof(list));
    init_list(env, NULL, List, NULL);
  } else {
    frame = env->kindData.listData;
  }

  /* if the env was NULL or the env was empty, set frame to the new node */
  if(frame == NULL)
    frame = node;
  else /* else cons the node onto the frame */
    frame = cons(node, frame);

  env->kindData.listData = frame;
  return env;
}

list *set_variable(char *var, list *value, list *env) {
  bind *binding = lookup_variable_binding(var, env);
  if(binding == NULL) {
    printf("ERROR undefined variable: %s\n", var);
    return NULL;
  }

  binding->value = value; // TODO: memory
  return env;
}

list *extend_environment(list *vars, list *vals, list *env) {
//  printf("vars: ");
//  print_sexp(vars);
//  printf("\n");
//
//  printf("vals: ");
//  print_sexp(vals);
//  printf("\n");

  list *frame = NULL;
  list *r, *l;
  for(r = vars, l = vals; r != NULL && l != NULL; r = r->next, l = l->next) {
    bind *binding = malloc(sizeof(bind));
    binding->name = r->kindData.atomData;
    binding->value = l->kindData.listData;
    list *frame_node = malloc(sizeof(list));
    init_list(frame_node, NULL, Bind, binding);
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
