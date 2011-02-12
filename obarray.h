#ifndef obarray_included
#define obarray_included
typedef struct symbol_t *symbol_t;
symbol_t intern(char *);
char *name(symbol_t);
#endif
