
#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf */
#include "list.h"

/* Append new node to the front of the list, returns new front.
   Can be called with NULL old, returns NULL if new is NULL. */
LIST *cons(LIST *old, LIST *new) {
  if(new == NULL)
    return NULL;
  new->next = old;
  return new;
}

/* Inserts new node right after the old node.
   Returns NULL if old or new is NULL */
LIST *insert(LIST *old, LIST *new) {
  if(old == NULL || new == NULL)
    return NULL;

  LIST *next = old->next;
  new->next = next;
  old->next = new;
  
  return old;
}

/* Walks down the list, appending new node to the very end.
   Returns the new node, returns NULL if old or new is NULL. */
LIST *append(LIST *old, LIST *new) {
  if(old==NULL || new == NULL)
    return NULL;

  LIST *n = old;
  LIST *nn = n->next;
  while(nn != NULL) {
    n = nn;
    nn = nn->next;
  }

  n->next = new;
  return new;
}

/* Pushes new node onto stack, mutating the stack pointer.
   Returns the new stack. Can be called on a pointer to NULL,
   ie this is ok:

   LIST *stack = NULL;
   push(&stack, new); */
LIST *push(LIST **pstack, LIST *new) {
  if(new == NULL)
    return NULL;

  if(*pstack == NULL) {
    *pstack = new;
  } else {
    new->next = *pstack;
    *pstack = new;
  }
  return new;
}

/* Pops from the stack, and mutates the stack pointer */
LIST *pop(LIST **pstack) {
  if(pstack == NULL || *pstack == NULL)
    return NULL;

  LIST *n = *pstack;
  *pstack = (*pstack)->next;
  return n;
}

/* Prints a list in box-and-pointer style */
void print_boxp(LIST *head) {
  LIST *n = head;
  while(n!=NULL) {
    if(n->type == ATOM) {
      printf("[%s]", (char*)n->data);
    } else if(n->type == PROC) {
      printf("[<proc>]");
    } else {
      printf("[ ");
      print_boxp(n->data);
      printf(" ]");
    }

    if(n->next!=NULL)
      printf("->");
    n = n->next;
  }
}

/* Prints a list as an s-expression, recursively printing nested lists */
void print_sexp(LIST *head) {
  LIST *n = head;
  printf("(");
  while(n != NULL) {
    if(n->type == ATOM) {
      printf("%s", (char*)n->data);
    } else {
      print_sexp((LIST*)n->data);
    }
    if(n->next != NULL)
      printf(" ");
    n = n->next;
  }
  printf(")");
}

/* Simple length of the list (nested lists count for 1) */
int length(LIST *head) {
  int len=0;
  for(LIST *n = head; n!=NULL; n=n->next)
    len++;
  return len;
}

/* Recursively counts the length of the list and all sublists */
int length_r(LIST *head) {
  int len=0;
  for(LIST *n = head; n != NULL; n = n->next) {
    if(n->type != CONS) {
      len++;
    } else {
      if(n->data != NULL)
        len+=length_r(n->data);
    }
  }
  return len;
}

/* recursively frees a list, but only frees ATOM data,
   so could would leak if PROCs were in the list */
void free_r(LIST *l) {
  LIST *n, *nn;
  for(n=l; n != NULL; ) {
    if(n->type == ATOM)
      free(n->data);
    else if(n->type == CONS)
      if(n->data != NULL)
        free_r(n->data);
    nn = n->next;
    free(n);
    n=nn;
  }
}

/* Calls the procedure "fn" for each node in the list */
void foreach(LIST *head, void(*fn)(LIST*)) {
  if(head == NULL)
    return;
  for(LIST *n = head; n != NULL; n = n->next) {
    fn(n);
  }
}
