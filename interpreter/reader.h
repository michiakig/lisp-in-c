#ifndef reader_included
#define reader_included
#include <stdio.h>
#include "storage.h"
object_t parse_sexp(char *);
char *read_sexp(FILE *);
object_t read_file(char *);
object_t read_stream(FILE *);
#endif
