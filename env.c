#include <string.h> /* for strcmp */
#include <stdio.h> /* for printf */
#include <stdlib.h> /* for malloc */
#include "env.h"
#include "llist.h"
#include "eval.h"

void print_frame(LLIST *frame);

/*
 * An environment is a list of frames. A frame is an LLIST of BINDs.
 * A BIND is a name, value binding pair defined as a struct, name is a char*
 * and value is a DATA*
 */
DATA *lookup_variable_value(char *variable, LLIST *env) {
  if(env == NULL) {
    printf("ERROR unknown variable: %s\n", variable);
    return NULL;
  } else {
    BIND *binding;

    for(LLIST *frame = (LLIST*)env->data; frame != NULL; frame = frame->next) {
      binding = (BIND*)frame->data;
      if(strcmp(variable, binding->name) == 0) {
        return binding->value;
      }
    }
    return lookup_variable_value(variable, env->next);
  }
}

LLIST *define_variable(char *var, DATA *value, LLIST *env) {
  /* allocate a new binding*/
  BIND *binding = malloc(sizeof(BIND));
  binding->name = var;
  binding->value = value;

  /* allocate a new node to be added to the frame */
  LLIST *node = malloc(sizeof(LLIST));
  node->data = binding;
  node->primitive = 1;
  node->next = NULL;

  /* get the frame, allocate a new env if needed */
  LLIST *frame = NULL;
  if(env == NULL) {
    env = malloc(sizeof(LLIST));
    env->primitive = 0;
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

void print_frame(LLIST *frame) {
  printf("(");
  for(LLIST *n = frame; n != NULL; n = n->next) {
    BIND *b = (BIND*)n->data;
    printf("(");
    printf("%s . ", b->name);
    print_data(b->value);
    printf(")");
    if(n->next != NULL)
      printf(" ");
  }
  printf(")");
}
