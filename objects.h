#ifndef object_included
#define object_included
typedef struct object_t *object_t
extern int object_consp(object_t);
extern int object_symbolp(object_t);
extern object_t object_newsymbol(char *);
#endif
