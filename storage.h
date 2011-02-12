#ifndef storage_included
#define storage_included

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

object_t obj_new_procedure(procedure_t);
object_t obj_new_symbol(char *);

void print_object(object_t);
char *obj_symbol_name(object_t);
procedure_t obj_getProcedure(object_t p);
int symbolcmp(object_t, object_t);

void user_print_object(object_t);

#endif
