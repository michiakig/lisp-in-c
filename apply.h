#ifndef apply_included
#define apply_included
#include "types.h"
procedure_t procedure_new(object_t, object_t, object_t, object_t (*) (object_t));
object_t primitive_add(object_t);
object_t primitive_multiply(object_t);
object_t primitive_subtract(object_t);
object_t primitive_divide(object_t);
object_t primitive_lessthan(object_t);
object_t primitive_greaterthan(object_t);
object_t primitive_equals(object_t);
object_t primitive_cdr(object_t);
object_t primitive_car(object_t);
object_t primitive_cons(object_t);
object_t apply(procedure_t, object_t);
#endif
