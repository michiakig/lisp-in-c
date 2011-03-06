#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../interpreter/apply.h"
#include "../interpreter/storage.h"
#include "../interpreter/obarray.h"
#include "../interpreter/types.h"
#include "runtime.h"

enum kind { Symbol, Cons, Procedure, String, Number };

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

#define HEAPSIZE 100000

#define STACKSIZE 100

object_t reg[NUM_REGISTERS] = {0};
object_t *stack;
object_t *sp;

object_t *the_heap;
object_t *new_heap;
int freeptr = NUM_REGISTERS;
int free_;
int scan;
object_t root = NULL;
object_t old;

void save(object_t x) {
  *sp = x; sp++;
}

object_t restore() {
  sp--; return *sp;
}

void init_heap() {
  the_heap = malloc(sizeof(*the_heap)*HEAPSIZE);
  new_heap = malloc(sizeof(*new_heap)*HEAPSIZE);
  int i;
  for(i=0; i<HEAPSIZE; i++) {
    the_heap[i] = NULL;
    new_heap[i] = NULL;
  }

  stack = malloc(sizeof(*stack)*STACKSIZE);
  for(i=0; i<STACKSIZE; i++)
    stack[i] = NULL;
  sp=stack;
}

/* garbage collection routine */
object_t relocate_old_result_in_new(object_t old) {
  object_t new = old; /* if it's not a cons, just return the data */
  /* if it's a pair, perform the relocation */
  if(iscons(old) && !isnull(old))
    if(!isbrokenheart(car(old))) { /* pair */
      new_heap[free_++] = car(old);
      new_heap[free_++] = cdr(old);
      new = malloc(sizeof(*new));
      new_heap[free_++] = new;
      new->type = Cons;
      set_car(new, car(old));
      set_cdr(new, cdr(old));

      set_car(old, brokenheart);
      set_cdr(old, new);

    } else /* already-moved */
      new = cdr(old);
  return new;
}

void stop_and_copy() {
  printf("running gc...");

  /* free, scan point to the start of new memory */
  free_ = NUM_REGISTERS;
  scan = NUM_REGISTERS;

  /* copy the registers into the special reserved portion of old memory */
  int i;
  the_heap[0] = malloc(sizeof(*the_heap[0]));
  for(i = 0; i<NUM_REGISTERS; i++) {
    the_heap[i]->type = Cons;
    set_car(the_heap[i], reg[i]);
    the_heap[i+1] = malloc(sizeof(*the_heap[i+1]));
    set_cdr(the_heap[i], reg[i]);
  }
  the_heap[i]->type = Cons;
  set_car(the_heap[i], reg[i]);
  set_cdr(the_heap[i], NIL);

  /* manually set up root at the beginning of new memory */
  new_heap[free_] = malloc(sizeof(*new_heap[free_]));
  new_heap[free_]->type = Cons;  
  set_car(new_heap[free_], the_heap[0]);
  set_cdr(new_heap[free_], NIL);
  root = new_heap[free_];
  free_ += 1;

  /* scan now points to a pair which has been relocated into the new
     memory, but the car, cdr of this pair have not been relocated
     yet */
  while(free_ != scan) { /* gc-loop */
    /* relocate the car, cdr of the pair pointed to by scan, and
       increment scan */
    old = new_heap[scan];
    set_car(old, relocate_old_result_in_new(car(old)));
    set_cdr(old, relocate_old_result_in_new(cdr(old)));
    scan++;
  }

  freeptr = free_;

  /* gc-flip */

  /* before flipping the memory -- copy the registers back out. */
  int r;
  for(r = 0; r<NUM_REGISTERS; r++)
    reg[r] = cdr(the_heap[r]); /* cdr holds the fwding address */


  /* flip the memory pointers */
  object_t *tmp = new_heap;
  new_heap = the_heap;
  the_heap = tmp;

  /* actually perform C free() calls on the garbage */
  for(i = 0; i<HEAPSIZE; i++)
    free(new_heap[i]);

  printf("gc done.\n");
}


/* old storage allocation and object functions below */

object_t obj_new() {
  if(freeptr < HEAPSIZE) {
    the_heap[freeptr] = malloc(sizeof(*the_heap[freeptr]));
    freeptr++;
    return the_heap[freeptr-1];
  } else
    printf("OUT OF MEMORY\n");
}

object_t obj_new_number(int n) {
  object_t new = obj_new();
  new->type = Number;
  new->data.numberData = n;
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


