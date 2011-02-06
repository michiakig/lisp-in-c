#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "types.h"
#include "list.h"

#include "eval.h"
#include "apply.h"

void init_proc(proc *p, list *params, list *body,
               list *env, list* (*fn) (list *argl)) {
  p->params = params;
  p->body = body;
  p->env = env;
  p->fn = fn;
}

list *primitive_add(list *argl) {

  double sum = 0;
  char *s;
  printf("primitive add ");
  print_sexp(argl);
  printf("\n");

  for(list *arg = argl; arg != NULL; arg = arg->next) {
    if(arg->type == Atom) {
      s = arg->kindData.atomData;

      sum += atoi(s);
    }
  }
  s = malloc(sizeof(char)*100); /* TODO: calculate the num digits */
  sprintf(s, "%f", sum);

  list *ret = malloc(sizeof(list));
  init_list(ret, NULL, Atom, s);

  return ret;
}

list *apply(proc *p, list *argl) {
  if(p->fn != NULL) { // primitive procedure
    return (*(p->fn))(argl);
  } else {
    // TODO: compound procedures
  }
}
