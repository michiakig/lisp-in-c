#ifndef __types__
#define __types__ 1

enum kind { Atom, List, Proc, Bind };

typedef struct list {
  struct list *next;
  enum kind type;
  union {
    struct list *listData;
    char *atomData;
    struct proc *procData;
    struct bind *bindData;
  } kindData;
} list;

typedef struct proc {
  struct list *params;
  struct list *body;
  struct list *env;
  struct list* (*fn) (list *argl);
} proc;

typedef struct bind {
  char *name;
  struct list *value;
} bind;

#endif
