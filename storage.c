#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "apply.h"
#include "storage.h"
#include "obarray.h"
#include "types.h"

enum kind { Symbol, Cons, Procedure };

struct object_t {
  enum kind type;
  union {
    int consData;
    symbol_t symbolData;
    procedure_t procData;
  } data;
};

struct object_t nildata = { .type = Cons, .data = { .consData = -1 } };
object_t NIL = &nildata;

#define MEMORY 1000 /* this is managed memory */
static object_t the_cars[MEMORY] = {NULL};
static object_t the_cdrs[MEMORY] = {NULL};
static int freeptr = 0;

int symbolcmp(object_t a, object_t b) {
  return a->data.symbolData == b->data.symbolData;
}

procedure_t obj_getProcedure(object_t p) {
  return p->data.procData;
}

object_t obj_new_procedure(procedure_t p) {
  object_t new = malloc(sizeof(*new));
  new->type = Procedure;
  new->data.procData = p;
  return new;
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

int procp(object_t obj) {
  return obj->type == Procedure;
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

void set_car(object_t obj, object_t new) {
  assert(obj->type == Cons);
  int i = obj->data.consData;
  assert(i < MEMORY);
  the_cars[i] = new;
}

void set_cdr(object_t obj, object_t new) {
  assert(obj->type == Cons);
  int i = obj->data.consData;
  assert(i < MEMORY);
  the_cdrs[i] = new;
}

/* prints an object_t, either a symbol or a cons (as an s-exp) */
void print_object(object_t obj) {

  switch(obj->type) {
  case Symbol:
    printf("%s", name(obj->data.symbolData));
    return;
    
  case Procedure:
    printf("<proc>");
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
      printf(" . %s", name(d->data.symbolData));
      i = -1;
      break;
    case Cons:
      i = d->data.consData;
      if(i != -1)
        printf(" ");
      break;
    case Procedure:
      printf(" . <proc>");
      i = -1;
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

