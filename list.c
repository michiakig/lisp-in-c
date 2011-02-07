#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf */

#include "types.h"
#include "list.h"

void init_list(list *node, list *next, enum kind type, void *data) {
  node->next = next;  
  node->type = type;
  if(type == Atom)
    node->kindData.atomData = (char*)data;
  else if(type == List)
    node->kindData.listData = (list*)data;
  else if(type == Proc)
    node->kindData.procData = (proc*)data;
  else if(type == Bind)
    node->kindData.bindData = (bind*)data;
}

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
  while(n->next != NULL) {
    n = n->next;
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

/* Calls the procedure "fn" for each node in the list */
void foreach(list *head, void(*fn)(list*)) {
  if(head == NULL)
    return;
  for(list *n = head; n != NULL; n = n->next) {
    fn(n);
  }
}

list *shallow_node_copy(list *orig) {
  list* copy = malloc(sizeof(list));
  copy->next = NULL;
  copy->type = orig->type;
  copy->kindData = orig->kindData;

  return copy;
}
