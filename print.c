#include <stdio.h>
#include "types.h"

void print_data(list *node) {
  if(node->type == Atom) {
    printf("%s", node->kindData.atomData);
  }

//else if(n->type == Proc) { // TODO fix procedure type to have some name
//                               // and print that
//    printf("<proc>");
//  } else if(n->type == Bind) { // TODO print the actual binding name, value
//    printf("<bind>");
//  }
}

/* Prints a list in a box-and-pointer style */
void print_boxp(list *head) {
  list *n = head;
  while(n != NULL) {
    printf("[");
    if(n->type != List)
      print_data(n);
    else
      print_boxp(n->kindData.listData);
    printf("]");

    if(n->next != NULL)
      printf("->");
    n = n->next;
  }
}

/* Prints a list as an s-expression, recursively printing nested lists */
void print_sexp(list *head) {
  list *n = head;
  printf("(");
  while(n != NULL) {
    if(n->type != List)
      print_data(n);
    else
      print_sexp(n->kindData.listData);

    if(n->next != NULL)
      printf(" ");
    n = n->next;
  }
  printf(")");
}

void print_exp(list *exp) {
  if(exp != NULL) {
    if(exp->type == Atom) {
      printf("%s", exp->kindData.atomData);
    } else if(exp->type == Proc) {
      printf("<proc>");
    } else if(exp->type == List){
      print_sexp(exp->kindData.listData);
    }
  }
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

void print_env(list *env) {
  printf("(");
  for(list *f = env; f != NULL; f = f->next) {
    print_frame(f->kindData.listData);
    printf("\n");
    if(f->next != NULL)
      printf(" ");
  }
  printf(")");
}
