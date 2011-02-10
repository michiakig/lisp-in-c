#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf */
#include <assert.h>

#include "types.h"
#include "list.h"

/* Prints a list in a box-and-pointer style */
void print_boxp(list *head) {
  list *n = head;
  while(n != NULL) {
    printf("[");
    if(n->type == Symbol)
      printf("%s", n->data.symbolData->name);
    else if(n->type == List)
      print_boxp(n->data.listData);
    printf("]");

    if(n->next != NULL)
      printf("->");
    n = n->next;
  }
}

void init_list(list *node, list *next, enum kind type, void *data) {
  node->next = next;  
  node->type = type;
  if(type == Symbol)
    node->data.symbolData = (symbol*)data;
  else if(type == List)
    node->data.listData = (list*)data;
  else if(type == Procedure)
    node->data.procData = (procedure*)data;
  else if(type == Binding)
    node->data.bindData = (binding*)data;
  else if(type == String)
    node->data.stringData = (char*)data;
}

/* Prepend new node to the front of the list, returns new front.
   Can be called with NULL old, returns NULL if new is NULL. */
list *prepend(list *new, list *old) {
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

void simple_rfree(list *l) {
  list *n = l;
  list *tmp = NULL;
  while(n != NULL) {
    if(n->type == String)
      free(n->data.stringData);
    else if(n->type == Binding) {
      simple_rfree(n->data.bindData->value);
      free(n->data.bindData);
    } else if(n->type == List)
      if(n->data.listData != NULL)
        simple_rfree(n->data.listData);

    tmp = n->next;
    free(n);
    n = tmp;
  }
}

/* Calls the procedure "fn" for each node in the list */
void foreach(list *head, void(*fn)(list*)) {
  list *n;
  if(head == NULL)
    return;
  for(n = head; n != NULL; n = n->next) {
    fn(n);
  }
}

list *shallow_node_copy(list *orig) {
  list* copy = malloc(sizeof(list));
  copy->next = NULL;
  copy->type = orig->type;
  copy->data = orig->data;

  return copy;
}

list *deep_list_copy(list *orig) {
  list *copy = NULL; /* head of the copy */
  list *last = NULL; /* last node in the copy */
  list *new;
  list *n;
  for(n = orig; n != NULL; n = n->next) {
    if(copy == NULL) {
      copy = malloc(sizeof(list));
      last = copy;
    } else {
      new = malloc(sizeof(list));
      last = append(new, last);
    }
    if(n->type == Symbol) {
      init_list(last, NULL, Symbol, n->data.symbolData);
    } else if(n->type == List) {
      init_list(last, NULL, List, deep_list_copy(n->data.listData));
    }
  }

  return copy;
}


symbol *getSymbol(list *l) {
  assert(l->type == Symbol);
  return l->data.symbolData;
}

list *getList(list *l) {
  assert(l->type == List);
  return l->data.listData;
}

procedure *getProc(list *l) {
  assert(l->type == Procedure);
  return l->data.procData;
}

binding *getBind(list *l) {
  assert(l->type == Binding);
  return l->data.bindData;
}

int getCons(list *l) {
  assert(l->type == Cons);
  return l->data.consData;
}
