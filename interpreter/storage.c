#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "storage.h"
#include "obarray.h"
#include "types.h"

enum kind { Symbol, Cons, Procedure, String };

struct object_t {
  enum kind type;
  union {
    struct {
      object_t car;
      object_t cdr;
    } consData;
    symbol_t symbolData;
    proc_t procData;
    char *stringData;
  } data;
};

/* special global variable for null type */
struct object_t empty = { .type = Cons,
                        .data = { .consData = { .car = NULL,
                                                .cdr = NULL } } };
object_t NIL = &empty;

#define HEAPSIZE 1000000 /* this is managed memory */

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

object_t obj_new_string(char *string) {
  char *copy = malloc(sizeof(*copy) * (strlen(string)+1));
  char *s, *c;
  for(c = copy, s = string; *s != '\0'; s++, c++)
    if(*s == '\\') {
      switch(*(s+1)) {
      case 'n':
        *c = '\n';
        break;
      case 't':
        *c = '\t';
        break;
      default:
        *c = *(s+1);
        break;
      }
      s++;
    } else
      *c = *s;

  *c = '\0';
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
  if(issymbol(obj))
    printf("%s", symbol_name(obj_get_symbol(obj)));
  else if(isstring(obj))
    printf("\"%s\"", obj_get_string(obj));
  else if(isproc(obj)) {
    /*    if(isprimitiveproc(obj_get_proc(obj))) */
      printf("<proc>");
      /*    else */
      /*      printf("<compound proc>"); */
  }

  else if(iscons(obj)) {
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
