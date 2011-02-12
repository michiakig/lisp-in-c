#ifndef env_included
#define env_included
#include "types.h"
object_t init_global();
object_t extend_environment(object_t, object_t, object_t);
object_t set_variable(object_t, object_t, object_t);
object_t define_variable(object_t, object_t, object_t);
object_t lookup_variable(object_t, object_t);
object_t assoc(object_t, object_t);
#endif
