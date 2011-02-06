#include <string.h> /* for strcmp */
#include <stdio.h> /* for printf */
#include <stdlib.h> /* for malloc */

#include "types.h"
#include "list.h"

#include "env.h"
#include "eval.h"
#include "apply.h"

void print_frame(list *frame);

list *init_global() {
  proc *add = malloc(sizeof(proc));
  init_proc(add, NULL, NULL, NULL, &primitive_add);

  list *value = malloc(sizeof(list));
  init_list(value, NULL, Proc, add);

  bind *b = malloc(sizeof(bind));
  b->name = "+";
  b->value = value;

  list *frame = malloc(sizeof(list));
  init_list(frame, NULL, Bind, b);

  list *env = malloc(sizeof(list));
  init_list(env, NULL, List, frame);
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
  if(binding == NULL)
    printf("ERROR undefined variable: %s\n", variable);


  return binding->value;
}

list *define_variable(char *var, list *value, list *env) {
  bind *binding = lookup_variable_binding(var, env);
  if(binding != NULL) {
    printf("ERROR variable already defined: %s\n", var);
    return NULL;
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

void print_frame(list *frame) {
  printf("(");
  for(list *n = frame; n != NULL; n = n->next) {
    bind *b = n->kindData.bindData;
    printf("(");
    printf("%s . ", b->name);
    print_exp(b->value);
    printf(")");
    if(n->next != NULL)
      printf(" ");
  }
  printf(")");
}
