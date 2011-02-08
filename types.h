#ifndef __types__
#define __types__ 1

enum kind { Symbol, List, Procedure, Binding, String };

typedef struct list {
  struct list *next;
  enum kind type;
  union {
    struct list *listData;
    struct symbol *symbolData;
    struct procedure *procData;
    struct binding *bindData;
    char *stringData;
  } data;
} list;

typedef struct procedure {
  struct list *params;
  struct list *body;
  struct list *env;
  struct list* (*fn) (list *argl);
} procedure;

typedef struct symbol {
  char *name;
} symbol;

typedef struct binding {
  symbol *name;
  struct list *value;
} binding;

#endif
