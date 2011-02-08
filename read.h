#ifndef __read__
#define __read__ 1

#include "types.h"
#include "hasht.h"

list *read_sexp(FILE *f);
list *parse_sexp(list *lines, struct nlist *hashtable[]);

#endif
