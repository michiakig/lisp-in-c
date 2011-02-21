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
    struct {
      object_t car;
      object_t cdr;
    } consData;
    symbol_t symbolData;
    proc_t procData;
  } data;
};

/* special global variable for null type */
struct object_t nil = { .type = Cons,
                        .data = { .consData = { .car = NULL,
                                                .cdr = NULL } } };
object_t NIL = &nil;

#define HEAPSIZE 10000 /* this is managed memory */
static object_t heap[HEAPSIZE] = {NULL};
static int freeptr = 0;
static object_t LAMBDA;

void init_heap() {
  LAMBDA = obj_new_symbol("_lambda");
}

object_t obj_new() {
  if(freeptr < HEAPSIZE) {
    heap[freeptr] = malloc(sizeof(*heap[freeptr]));
    freeptr++;
    return heap[freeptr-1];
  } else {
    printf("OUT OF MEMORY\n");
  }
}

int obj_symbol_cmp(object_t a, object_t b) {
  return a->data.symbolData == b->data.symbolData;
}

proc_t obj_get_proc(object_t p) {
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

  /* this is essentially a memory leak */
  object_t new = obj_new();
  new->type = Symbol;
  new->data.symbolData = symbol;
  return new;
}

symbol_t obj_get_symbol(object_t obj) {
  return obj->data.symbolData;
}

int lambdap(object_t obj) {
  return consp(obj) && !nilp(obj) && obj_symbol_cmp(car(obj), LAMBDA);
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
  return obj == NIL;
}

object_t cons(object_t a, object_t d) {
  object_t new = obj_new();
  new->type = Cons;
  new->data.consData.car = a;
  new->data.consData.cdr = d;
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
  if(symbolp(obj))
    printf("%s", symbol_name(obj_get_symbol(obj)));
  else if(procp(obj))
    printf("<proc>");
  else if(lambdap(obj))
    printf("<compound proc>");
  else if(nilp(obj))
    printf("nil");
  else if(consp(obj)) {
    printf("(");
    while(!nilp(obj)) {
      print_object(car(obj));
      if(!nilp(cdr(obj)))
        printf(" ");
      if(!consp(cdr(obj))) {
        printf(". ");
        print_object(cdr(obj));
        obj = NIL;
      } else
        obj = cdr(obj);
    }
    printf(")");
  }
}

object_t storage_append(object_t new, object_t old) {
  while(!nilp(cdr(old))) {
    old = cdr(old);
  }
  set_cdr(old, cons(new, NIL));
  return cdr(old);
}
