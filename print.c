#include <stdio.h>
#include "types.h"
#include "hasht.h"
#include "storage.h"

void print_data(list *node) {
  if(node->type == Symbol)
    printf("%s", node->data.symbolData->name);
  else if(node->type == Procedure) // TODO fix procedure type to have some name
    printf("<proc>");
  else if(node->type == Cons)
    print_cons(node);
  else
    printf("UNKNOWN");
}

/* Prints a list in a box-and-pointer style */
void print_boxp(list *head) {
  list *n = head;
  while(n != NULL) {
    printf("[");
    if(n->type != List)
      print_data(n);
    else
      print_boxp(n->data.listData);
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
      print_sexp(n->data.listData);

    if(n->next != NULL)
      printf(" ");
    n = n->next;
  }
  printf(")");
}

void print_exp(list *exp) {
  if(exp != NULL)
     if(exp->type == List)
      print_sexp(exp->data.listData);
    else
       print_data(exp);
  else
    printf("NULL");
}

void print_frame(list *frame) {
  printf("(");
  for(list *n = frame; n != NULL; n = n->next) {
    binding *b = n->data.bindData;
    printf("(");
    printf("%s . ", b->name->name);
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
    print_frame(f->data.listData);
    printf("\n");
    if(f->next != NULL)
      printf(" ");
  }
  printf(")");
}

void print_hashtable(struct nlist *hashtable[], int len) {
  int i;
  struct nlist *np;
  for (i = 0; i < len; i++) {
    if(hashtable[i] != NULL) {
      printf("[%d]", i);
      for (np = hashtable[i]; np != NULL; np = np->next) {
        printf("->{%s}", np->sym->name);
      }
      printf("\n");
    }
  }
}
