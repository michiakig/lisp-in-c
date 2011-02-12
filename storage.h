#ifndef storage_included
#define storage_included
typedef struct object_t *object_t;
extern object_t NIL;
object_t cons(object_t, object_t);
object_t cdr(object_t);
object_t car(object_t);
object_t storage_append(object_t, object_t);
int nilp(object_t);
int consp(object_t);
int symbolp(object_t);
object_t obj_new_symbol(char *);
void print_object(object_t);
char *obj_symbol_name(object_t);
int symbolcmp(object_t, object_t);
#endif
