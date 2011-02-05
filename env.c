#include <string.h> /* for strcmp */
#include <stdio.h> /* for printf */
#include <stdlib.h> /* for malloc */

#include "env.h"
#include "eval.h"
#include "list.h"

void print_frame(LIST *frame);

/*
 * An environment is a list of frames. A frame is an LIST of BINDINGs.
 * A BINDING is a name, value binding pair defined as a struct, name is a char*
 * and value is a LIST*
 */
LIST *lookup_variable_value(char *variable, LIST *env) {
  if(env == NULL) {
    printf("ERROR unknown variable: %s\n", variable);
    return NULL;
  } else {
    BINDING *binding;
    for(LIST *frame = (LIST*)env->data; frame != NULL; frame = frame->next) {
      binding = (BINDING*)frame->data;
      if(strcmp(variable, binding->name) == 0)
        return binding->value;
    }
    return lookup_variable_value(variable, env->next);
  }
}

LIST *define_variable(char *var, LIST *value, LIST *env) {
  /* allocate a new binding*/
  BINDING *binding = malloc(sizeof(BINDING));
  binding->name = var;
  binding->value = value;

  /* allocate a new node to be added to the frame */
  LIST *node = malloc(sizeof(LIST));
  node->data = binding;
  node->type = BIND;
  node->next = NULL;

  /* get the frame, allocating a new env if needed */
  LIST *frame = NULL;
  if(env == NULL) {
    env = malloc(sizeof(LIST));
    env->type = CONS;
    env->data = NULL;
  } else {
    frame = env->data;
  }

  /* if the env was NULL or the env was empty, set frame to the new node */
  if(frame == NULL) {
    frame = node;
  } else { /* else cons the node onto the frame */
    node->next = frame;
    frame = node;
  }

  print_frame(frame);
  printf("\n");

  env->data = frame;
  return env;
}

void print_frame(LIST *frame) {
  printf("(");
  for(LIST *n = frame; n != NULL; n = n->next) {
    BINDING *b = (BINDING*)n->data;
    printf("(");
    printf("%s . ", b->name);
    print_data(b->value);
    printf(")");
    if(n->next != NULL)
      printf(" ");
  }
  printf(")");
}
