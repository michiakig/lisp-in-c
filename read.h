#ifndef __read__
#define __read__ 1

#include "aux.h"
#include "hasht.h"

struct node *read_sexp(FILE *f);
list *parse_sexp(struct node *lines, struct nlist *obarray[]);

#endif
