
#ifndef __eval__
#define __eval__

#include "list.h"

void print_data(LIST *d);
LIST *eval(LIST *expr, LIST **env);
LIST *eval_define(LIST *expr, LIST **env);
LIST *eval_appl(LIST *exp, LIST **env);

#endif
