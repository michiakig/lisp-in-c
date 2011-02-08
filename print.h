#ifndef __print__
#define __print__ 1

#include "hasht.h"

void print_sexp(list *head);
void print_boxp(list *head);
void print_exp(list *exp);
void print_frame(list *frame);
void print_env(list *env);

void print_hashtable(struct nlist *hashtable[], int len);

#endif
