#ifndef apply_included
#define apply_included
#include "types.h"

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
object_t primitive_set_cdr(object_t);
object_t primitive_set_car(object_t);
object_t primitive_eq(object_t);
object_t primitive_isnull(object_t);

object_t primitive_read_file(object_t);
object_t primitive_load(object_t);
object_t primitive_print(object_t);

object_t primitive_apply(object_t);
object_t primitive_eval(object_t);
object_t primitive_read(object_t);
object_t primitive_quit(object_t);
object_t primitive_error(object_t);

object_t primitive_numberp(object_t);
object_t primitive_stringp(object_t);
object_t primitive_symbolp(object_t);
object_t primitive_consp(object_t);

int isprimitiveproc(proc_t);
int iscompoundproc(proc_t);
object_t obj_new_compound(object_t, object_t, object_t);
object_t obj_new_primitive(object_t (*) (object_t));
object_t apply(object_t, object_t);
#endif
