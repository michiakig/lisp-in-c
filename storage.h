#ifndef storage_included
#define storage_included
#include "obarray.h"
#include "types.h"

extern object_t NIL;
object_t cons(object_t, object_t);
object_t cdr(object_t);
object_t car(object_t);
void set_car(object_t, object_t);
void set_cdr(object_t, object_t);

object_t storage_append(object_t, object_t);

int nilp(object_t);
int consp(object_t);
int symbolp(object_t);
int procp(object_t);

proc_t obj_get_proc(object_t p);
object_t obj_new_proc(proc_t);

object_t obj_new_symbol(char *);
symbol_t obj_get_symbol(object_t);

void print_object(object_t);
int obj_symbol_cmp(object_t, object_t);
void user_print_object(object_t);

#endif
