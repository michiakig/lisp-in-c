#ifndef __LLIST__
#define __LLIST__ 1

typedef struct node {
  void *data;
  struct node *next; /* pointer to next element in list */
  int primitive;
} LLIST;
 
LLIST *list_add(LLIST **p, void *data);  /* Function definition to add an element */
void list_remove(LLIST **p);        /* Function definition to remove element */

LLIST *list_insert(LLIST *p, void *data);
LLIST *list_append(LLIST *p, void *data, int primitive);

void *pop(LLIST **stack);
LLIST *push(LLIST **stack, void *data, int primitive);

void list_print(LLIST *l);
void list_print_r(LLIST *l, int indent);
void list_print_sexp(LLIST *l);

int length_simple(LLIST *l);
int length_recur(LLIST *l);

void free_recur(LLIST *l);

#endif
