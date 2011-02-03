#ifndef __read__
#define __read__ 1

#include "eval.h"

LLIST *read_sexp(FILE *f);
DATA *parse_sexp(LLIST *lines);

#endif
