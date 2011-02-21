#ifndef __aux__
#define __aux__ 1

struct node *auxappend(struct node *new, struct node *lst);
/*
void auxpush(struct node *new, struct node **stack);
struct node *auxpop(struct node **stack);
*/
void auxfor_each(struct node *n, void (*fn) (struct node*));
void auxprint_node(struct node *n);
void auxfree_node(struct node *n);

struct node {
  struct node *next;
  void *data;
};

#endif
