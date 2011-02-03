
#ifndef __eval__
#define __eval__

#include "llist.h"

enum datatype {
  ATOM,
  LIST,
  PROC
};

typedef struct data {
  void *data;
  enum datatype type;
} DATA;

DATA *eval(DATA *expr, LLIST **env);
DATA *eval_define(DATA *expr, LLIST **env);
DATA *eval_appl(DATA *expr, LLIST **env);

void print_data(DATA *data);

#endif
