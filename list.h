#ifndef __LIST__
#define __LIST__ 1

/* In this linked-list implementation, nodes contain type information in the form of the following enum: */
enum datatype {
  ATOM, // Lisp atom, for now this is a char*
  CONS, // Nested list, in this case LIST*
  PROC,
  BIND
};

typedef struct node {
  void *data;
  enum datatype type;
  struct node *next;
} LIST;

LIST *cons(LIST *list, LIST *new);
LIST *insert(LIST *p, LIST *new);
LIST *append(LIST *p, LIST *new);
LIST *push(LIST **stack, LIST *new);
LIST *pop(LIST **stack);
void print_boxp(LIST *l);
void print_sexp(LIST *head);
int length_r(LIST *l);
int length(LIST *l);
void free_r(LIST *l);
void foreach(LIST *head, void(*fun)(LIST*));

#endif
