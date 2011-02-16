#ifndef obarray_included
#define obarray_included
typedef struct symbol_t *symbol_t;
symbol_t symbol_intern(char *);
char *symbol_name(symbol_t);
#endif
