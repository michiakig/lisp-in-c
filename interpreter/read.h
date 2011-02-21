#ifndef read_included
#define read_included
#include "storage.h"
#include "aux.h"

struct node *read_sexp(FILE *f);
object_t parse_sexp(struct node *lines);

#endif
