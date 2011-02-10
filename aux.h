
struct node *append(struct node *new, struct node *lst);
void push(struct node *new, struct node **stack);
struct node *pop(struct node **stack);
void for_each(struct node *n, void (*fn) (struct node*));
void print_node(struct node *n);
void free_node(struct node *n);

struct node {
  struct node *next;
  void *data;
};
