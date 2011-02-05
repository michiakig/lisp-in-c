#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "llist.h"
#include "eval.h"
#include "apply.h"

//int main(int argc, char **argv) {
//  PROCEDURE *p = malloc(sizeof(PROCEDURE));
//  p->fun = (DATA* (*) (LLIST*))primitive_sum;
//  LLIST *a = malloc(sizeof(LLIST));
//  DATA d[2];
//
//  a->next = NULL;
//  a->primitive = 1;
//  d[0].type = ATOM;
//  d[0].data = "1";
//  a->data = &d[0];
//
//  d[1].type = ATOM;
//  d[1].data = "2";
//
//  list_append(a, &d[1], 1);
//
//  DATA *ret = apply(p, a);
//  print_data(ret);
//  printf("\n");
//}

LIST *primitive_add(LIST *argl) {
  double sum=0;
  char *s;
  for(LIST *n=argl; n != NULL; n = n->next) {
    if(n->data != NULL && arg->type == ATOM) {
      s = (char*)arg->data;
      sum += atoi(s);
    }
  }
  s = malloc(sizeof(char)*100); // TODO calculate the num digits
  sprintf(s, "%f", sum);

  LIST *d = malloc(sizeof(LIST));
  d->type = ATOM;
  d->data = s;
  return d;
}

LIST *apply(PROCEDURE *proc, LLIST *argl) {
  if(proc->fun != NULL) { // primitive procedure
    return (*(proc->fun))(argl);
  } else {
    //
  }
}
