#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf */

#include "list.h"

/* Append new node to the front of the list, returns new front.
   Can be called with NULL old, returns NULL if new is NULL. */
list *cons(list *new, list *old) {
  if(new == NULL)
    return NULL;

  new->next = old;
  return new;
}

/* Inserts new node right after the old node.
   Returns NULL if old or new is NULL */
list *insert(list *new, list *old) {
  if(new == NULL || old == NULL)
    return NULL;

  list *next = old->next;
  new->next = next;
  old->next = new;
  
  return old;
}

/* Walks down the list, appending new node to the very end.
   Returns the new node, returns NULL if old or new is NULL. */
list *append(list *new, list *old) {
  if(new == NULL || old == NULL)
    return NULL;

  list *n = old;
  list *nn = n->next;
  while(nn != NULL) {
    n = nn;
    nn = nn->next;
  }

  n->next = new;
  return new;
}

/* Pushes new node onto stack, mutating the stack pointer.
   Returns the new stack. Can be called on a pointer to NULL */
list *push(list *new, list **pstack) {
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
list *pop(list **pstack) {
  if(pstack == NULL || *pstack == NULL)
    return NULL;

  list *n = *pstack;
  *pstack = (*pstack)->next;
  return n;
}

void print_data(list *node) {
  if(node->type == Atom) {
    printf("%s", node->kindData.atomData);
  }

//else if(n->type == Proc) { // TODO fix procedure type to have some name
//                               // and print that
//    printf("<proc>");
//  } else if(n->type == Bind) { // TODO print the actual binding name, value
//    printf("<bind>");
//  }
}

/* Prints a list in a box-and-pointer style */
void print_boxp(list *head) {
  list *n = head;
  while(n != NULL) {
    printf("[");
    if(n->type != List)
      print_data(n);
    else
      print_boxp(n->kindData.listData);
    printf("]");

    if(n->next != NULL)
      printf("->");
    n = n->next;
  }
}

/* Prints a list as an s-expression, recursively printing nested lists */
void print_sexp(list *head) {
  list *n = head;
  printf("(");
  while(n != NULL) {
    if(n->type != List)
      print_data(n);
    else
      print_sexp(n->kindData.listData);

    if(n->next != NULL)
      printf(" ");
    n = n->next;
  }
  printf(")");
}

///* Simple length of the list (nested lists count for 1) */
//int length(list *head) {
//  int len=0;
//  for(list *n = head; n!=NULL; n=n->next)
//    len++;
//  return len;
//}
//
///* Recursively counts the length of the list and all sublists */
//int length_r(list *head) {
//  int len=0;
//  for(list *n = head; n != NULL; n = n->next) {
//    if(n->type != CONS) {
//      len++;
//    } else {
//      if(n->data != NULL)
//        len+=length_r(n->data);
//    }
//  }
//  return len;
//}
//

/* recursively frees a list, but only frees ATOM data,
   so could would leak if PROCs were in the list */
void simple_rfree(list *l) {
  list *n = l;
  list *tmp;
  while(n != NULL) {
    if(n->type == Atom)
      free(n->kindData.atomData);
    else if(n->type == List)
      if(n->kindData.listData != NULL)
        simple_rfree(n->kindData.listData);

    tmp = n->next;
    free(n);
    n = tmp;
  }
}

///* Calls the procedure "fn" for each node in the list */
//void foreach(list *head, void(*fn)(list*)) {
//  if(head == NULL)
//    return;
//  for(list *n = head; n != NULL; n = n->next) {
//    fn(n);
//  }
//}
