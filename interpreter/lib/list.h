#ifndef list_included
#define list_included
struct list_t *list_push(void *, struct list_t *);
struct list_t *list_reverse(struct list_t *);
struct list_t *list_new(void *);
struct list_t *list_append(void *, struct list_t *);
void list_for_each(struct list_t *, void (*) (struct list_t*));
void list_print_node(struct list_t *);
void list_free_node(struct list_t *);
struct list_t {
  struct list_t *next;
  char *data;
  int len;
};
#endif
