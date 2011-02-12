#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "storage.h"
#include "obarray.h"

enum kind { Symbol, Cons };

struct object_t {
  enum kind type;
  union {
    int consData;
    symbol_t symbolData;
  } data;
};

struct object_t nildata = { .type = Cons, .data = { .consData = -1 } };
object_t NIL = &nildata;

#define MEMORY 10 /* this is managed memory */
static object_t the_cars[MEMORY] = {NULL};
static object_t the_cdrs[MEMORY] = {NULL};
static int freeptr = 0;

int symbolcmp(object_t a, object_t b) {
  return a->data.symbolData == b->data.symbolData;
}

object_t obj_new_symbol(char *s) {
  symbol_t symbol = intern(s);

  /* this is essentially a memory leak */
  object_t new = malloc(sizeof(*new));
  new->type = Symbol;
  new->data.symbolData = symbol;
  return new;
}

char *obj_symbol_name(object_t obj) {
  return name(obj->data.symbolData);
}

int symbolp(object_t obj) {
  return obj->type == Symbol;
}

int consp(object_t obj) {
  return obj->type == Cons;
}

int nilp(object_t obj) {
  if(obj->type == Cons && obj->data.consData == -1)
    return 1;
  else
    return 0;
}

object_t cons(object_t a, object_t d) {
  if(freeptr < MEMORY) {
    the_cars[freeptr] = a;
    the_cdrs[freeptr] = d;
    int index = freeptr;
    freeptr++;

    /* this is a memory leak, but I don't know how to fix it */
    object_t ret = malloc(sizeof(*ret));
    ret->type = Cons;
    ret->data.consData = index;
    return ret;
  } else {
    printf("OUT OF MEMORY\n");
  }
}

object_t car(object_t obj) {
  assert(obj->type == Cons);
  int i = obj->data.consData;
  assert(i < MEMORY);
  return the_cars[i];
}

object_t cdr(object_t obj) {
  assert(obj->type == Cons);
  int i = obj->data.consData;
  assert(i < MEMORY);
  return the_cdrs[i];
}

/* prints an object_t, either a symbol or a cons (as an s-exp) */
void print_object(object_t obj) {
  if(obj->type == Symbol) {
    printf("%s", name(obj->data.symbolData));
    return;
  }
  /* assume it's a cons */
  assert(obj->type == Cons);
  printf("(");
  int i = obj->data.consData;
  object_t a;
  object_t d;
  while(i != -1) {
    a  = the_cars[i];
    d  = the_cdrs[i];
    print_object(a);
    switch(d->type) {
    case Symbol:
      printf(" . %s)", name(d->data.symbolData));
      break;
    case Cons:
      i = d->data.consData;
      if(i != -1)
        printf(" ");
      break;
    }
  }
  printf(")");
}

object_t storage_append(object_t new, object_t old) {
  while(!nilp(cdr(old))) {
    old = cdr(old);
  }
  the_cdrs[old->data.consData] = cons(new, NIL);
  return cdr(old);
}

/*void storage_push(object_t new, object_t stack) {
  
  }*/
