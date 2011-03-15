#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../interpreter/apply.h"
#include "../interpreter/storage.h"
#include "../interpreter/obarray.h"
#include "../interpreter/types.h"
#include "runtime.h"

struct free_list_node {
  struct free_list_node *next;
  int index;
  int count;
};

enum kind { Symbol, Cons, Procedure, String, Number };

struct object_t {
  enum kind type;
  int marked;
  union {
    struct {
      object_t car;
      object_t cdr;
    } consData;
    symbol_t symbolData;
    proc_t procData;
    char *stringData;
    int numberData;
  } data;
};

/* special global variable for null type */
struct object_t empty = { .type = Cons,
                        .data = { .consData = { .car = NULL,
                                                .cdr = NULL } } };
object_t NIL = &empty;

/* special global variable for broken heart */
struct object_t brokenheartdata = { .type = Cons,
                                    .data = { .consData = { .car = NULL,
                                                            .cdr = NULL } } };
object_t brokenheart = &brokenheartdata;
int isbrokenheart(object_t hrt) {
  return hrt == brokenheart;
}

#define HEAPSIZE 400

#define STACKSIZE 50

object_t reg[NUM_REGISTERS] = {NULL};
object_t *stack;
object_t *sp;

object_t *heap;
int freeptr = 0;

struct free_list_node *free_list;

void save(object_t x) {
  *sp = x; sp++;
}

object_t restore() {
  sp--; return *sp;
}

void init_heap() {
  heap = malloc(sizeof(*heap)*HEAPSIZE);
  int i;
  for(i=0; i<HEAPSIZE; i++)
    heap[i] = NULL;

  free_list = malloc(sizeof(*free_list));
  free_list->next = NULL;
  free_list->index = 0;
  free_list->count = HEAPSIZE;

  stack = malloc(sizeof(*stack)*STACKSIZE);
  for(i=0; i<STACKSIZE; i++)
    stack[i] = NULL;
  sp=stack;
}


int ismarked(object_t o) {
  return o->marked;
}

void setcleared(object_t o) {
  o->marked=0;
}

void setmarked(object_t o) {
  o->marked=1;
}

void mark_starting_at(object_t root) {
  if(iscons(root) && !isnull(root)) {
    setmarked(root);
    if(!isnull(car(root)) &&
       car(root) != NULL &&
       !ismarked(car(root)))
       mark_starting_at(car(root));
    if(!isnull(cdr(root)) && cdr(root) != NULL && !ismarked(cdr(root)))
       mark_starting_at(cdr(root));
  } else
    setmarked(root);
}

void mark_and_sweep() {
  int i;
  for(i=0; i<NUM_REGISTERS; i++) {
    if(reg[i] != NULL)
      mark_starting_at(reg[i]);
  }
  for(i=0; i<STACKSIZE && stack[i]!=NULL; i++) {
    if(stack[i] != NULL)
      mark_starting_at(stack[i]);
  }

  for(i=0; i<HEAPSIZE; i++) {
    if(heap[i]==NULL)
      printf(".");
    else if(ismarked(heap[i]))
      printf("x");
    else
      printf("_");
  }
  printf("\n");

  /* free list is null at this point */
  free_list = malloc(sizeof(*free_list));
  free_list->index = -1;

  for(i=0; i<HEAPSIZE; i++) {
    if(heap[i]!=NULL && !ismarked(heap[i])) {
      /*      printf("freeing %d ", i); print_object(heap[i]);
              printf("\n"); */
      free(heap[i]);
      heap[i]=NULL;
      if(free_list->index < 0) {
        free_list->index = i;
        free_list->count = 1;
      } else
        free_list->count++;
    } else {
      if(heap[i]!=NULL)
        setcleared(heap[i]);
      if(free_list->index > 0) {
        struct free_list_node *f = malloc(sizeof(*f));
        f->next = free_list;
        f->index = -1;
        f->count = 0;
        free_list = f;
      }
    }
  }/*
  struct free_list_node *n;
  for(n = free_list; n; n=n->next)
    printf("%d,%d ", n->index, n->count);
    printf("\n");*/
}

object_t obj_new() {
  struct free_list_node *n;
  /*
  for(n = free_list; n; n=n->next)
    printf("%d,%d ", n->index, n->count);
  printf("\n");
  */
  while(free_list != NULL && free_list->count == 0) {
    struct free_list_node *f = free_list->next;
    free(free_list);
    free_list = f;
  }
  if(free_list == NULL) {
    /*    printf("running gc..."); */
    mark_and_sweep();
    /*    printf("gc done.\n"); */
    return obj_new();
  }
  /*  printf("allocing %d ", free_list->index); */
  heap[free_list->index] = malloc(sizeof(*heap[free_list->index]));
  object_t ret = heap[free_list->index];
  free_list->index++;
  free_list->count--;
  return ret;
}

/* old storage allocation and object functions below */

object_t obj_new_number(int n) {
  object_t new = obj_new();
  new->type = Number;
  new->data.numberData = n;
  /*  print_object(new); printf("\n"); */
  return new;
}

int obj_get_number(object_t obj) {
  assert(obj->type == Number);
  return obj->data.numberData;
}

int obj_symbol_cmp(object_t a, object_t b) {
  assert(a->type == Symbol && b->type == Symbol);
  return a->data.symbolData == b->data.symbolData;
}

proc_t obj_get_proc(object_t p) {
  assert(p->type == Procedure);
  return p->data.procData;
}

object_t obj_new_proc(proc_t p) {
  object_t new = obj_new();
  new->type = Procedure;
  new->data.procData = p;
  return new;
}

object_t obj_new_symbol(char *s) {
  symbol_t symbol = symbol_intern(s);
  object_t new = obj_new();
  new->type = Symbol;
  new->data.symbolData = symbol;
  /*  print_object(new); printf("\n"); */
  return new;
}

char *unescape(char *s) {
  char *copy = malloc(sizeof(*copy) * (strlen(s)+1));
  char *c;
  for(c = copy; *s != '\0'; c++)
    if(*s == '\\') {
      s++;
      if(0==strncmp(s, "newline", 7)) {
        *c = '\n';
        s = s+7;
      } else if(0==strncmp(s, "quote", 5)) {
        *c = '"';
        s = s+5;
      } else
        *c = '\\';
    } else {
      *c = *s;
      s++;
    }
  *c = '\0';
  return copy;
}

object_t obj_new_string(char *string) {
  char *copy = unescape(string);
  object_t obj = obj_new();
  obj->type = String;
  obj->data.stringData = copy;
  return obj;
}

symbol_t obj_get_symbol(object_t obj) {
  return obj->data.symbolData;
}

char *obj_get_string(object_t obj) {
  assert(isstring(obj));
  return obj->data.stringData;
}

int isnum(object_t obj) {
  return obj->type == Number;
}

int isstring(object_t obj) {
  return obj->type == String;
}

int issymbol(object_t obj) {
  return obj->type == Symbol;
}

int iscons(object_t obj) {
  return obj->type == Cons;
}

int isproc(object_t obj) {
  return obj->type == Procedure;
}

int isnull(object_t obj) {
  return obj == NIL;
}

object_t cons(object_t a, object_t d) {
  save(a);
  save(d);
  object_t new = obj_new();
  new->type = Cons;
  new->data.consData.cdr = restore();
  new->data.consData.car = restore();

  return new;
}

object_t car(object_t obj) {
  assert(obj->type == Cons);
  return obj->data.consData.car;
}

object_t cdr(object_t obj) {
  assert(obj->type == Cons);
  return obj->data.consData.cdr;
}

void set_car(object_t obj, object_t new) {
  assert(obj->type == Cons);
  obj->data.consData.car = new;
}

void set_cdr(object_t obj, object_t new) {
  assert(obj->type == Cons);
  obj->data.consData.cdr = new;
}

/* prints an object_t, either a symbol or a cons (as an s-exp) */
void print_object(object_t obj) {
  if(isnum(obj))
    printf("%d", obj_get_number(obj));
  else if(issymbol(obj))
    printf("%s", symbol_name(obj_get_symbol(obj)));
  else if(isstring(obj))
    printf("\"%s\"", obj_get_string(obj));
  else if(isproc(obj)) {
    /*    if(isprimitiveproc(obj_get_proc(obj))) */
      printf("<proc>");
      /*    else */
      /*      printf("<compound proc>"); */
  } else if(iscons(obj)) {
    printf("(");
    while(!isnull(obj)) {
      print_object(car(obj));
      if(!isnull(cdr(obj)))
        printf(" ");
      if(!iscons(cdr(obj))) {
        printf(". ");
        print_object(cdr(obj));
        obj = NIL;
      } else
        obj = cdr(obj);
    }
    printf(")");
  } else {
    printf("unknown");
  }
}

int isproperlist(object_t list) {
  while(iscons(cdr(list)) && !isnull(cdr(list)))
    list = cdr(list);
  if(isnull(cdr(list)))
    return 1;
  else
    return 0;
}

object_t storage_last(object_t list) {
  while(!isnull(cdr(list)))
    list = cdr(list);
  return list;
}

object_t storage_append(object_t new, object_t old) {
  old = storage_last(old);
  set_cdr(old, cons(new, NIL));
  return cdr(old);
}


