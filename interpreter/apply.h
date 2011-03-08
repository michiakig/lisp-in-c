#ifndef apply_included
#define apply_included
#include "types.h"

object_t init_global();
object_t load_file(char *, object_t *);

int isprimitiveproc(proc_t);
int iscompoundproc(proc_t);
object_t obj_new_compound(object_t, object_t, object_t);
object_t obj_new_primitive(object_t (*) (object_t));
object_t apply(object_t, object_t);
#endif
