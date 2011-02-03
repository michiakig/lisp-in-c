/*
 * originally based on code from Wikipedia:
 * http://en.wikipedia.org/wiki/Linked_list#Language_support
 * this as since been removed from the article
 */

#include <stdlib.h>   /* for malloc */
#include <stdio.h>
#include "llist.h"

void print_indent(int indent);
 
LLIST *list_add(LLIST **p, void *data) {
  if (p == NULL) // sanity check
    return NULL;
  LLIST *n = malloc(sizeof(LLIST)); // new node 
  if (n == NULL)
    return NULL;
 
  n->next = *p; // the previous element (*p) now becomes the "next" element
  *p = n;       // add new empty element to the front (head) of the list
  n->data = data;
 
  return *p;
}
 
void list_remove(LLIST **p) {
  if (p != NULL && *p != NULL) {
      LLIST *n = *p;
      *p = (*p)->next;
      free(n);
    }
}

LLIST *list_insert(LLIST *p, void *data) {
  if(p==NULL)
    return NULL;
  
  LLIST *n = malloc(sizeof(LLIST));
  if(n==NULL)
    return NULL;

  n->data = data;
  n->next = p->next;
  p->next = n;
  return n;
}

LLIST *list_append(LLIST *p, void *data, int primitive) {
  if(p==NULL)
    return NULL;

  LLIST *n = p;
  LLIST *nn = n->next;
  while(nn != NULL) {
    n = nn;
    nn = nn->next;
  }

  LLIST *d = (LLIST*)malloc(sizeof(LLIST));
  d->next = NULL;
  d->data = data;
  d->primitive = primitive;
  n->next = d;

  return d;
}

LLIST *push(LLIST **stack, void *data, int primitive) { 
  LLIST *next;
  if(*stack == NULL) {
    *stack = (LLIST*)malloc(sizeof(LLIST));
    (*stack)->primitive = primitive;
    (*stack)->data = data;
    (*stack)->next = NULL;
  } else {
    next = (LLIST*)malloc(sizeof(LLIST));
    next->next = (*stack);
    next->data = data;
    next->primitive = primitive;
    (*stack) = next;
  }
  return next;
}

void *pop(LLIST **stack) {
  LLIST *n = *stack;
  void *data = n->data;
  *stack = (*stack)->next;
  free(n);
  return data;
}

void list_print(LLIST *l) {
  LLIST *n = l;
  while(n!=NULL) {
    if(n->primitive)
      printf("[%s]", (char*)n->data);
    else
      printf("[...]");

    if(n->next!=NULL) {
      printf("->");
    }
    n = n->next;
  }
  printf("\n");
}

void list_print_r(LLIST *l, int indent) {
  //  printf("list_print_r len=%d\n", length_simple(l));
  LLIST *n = l;
  while(n != NULL) {
    if(n->primitive == 1) {
      print_indent(indent);
      printf("%s\n", (char*)n->data);
    } else {
      // non-primitive, but data is NULL
      if(n->data == NULL) {
        print_indent(indent);
        printf("()\n");
      } else {
        list_print_r(n->data, indent+2);
      }
    }
    n = n->next;
  }
}

void list_print_sexp(LLIST *l) {
  LLIST *n = l;
  printf("(");
  while(n != NULL) {
    if(n->primitive == 1) {
      printf("%s", (char*)n->data);
    } else {
      printf(" ");
      list_print_sexp((LLIST*)n->data);
    }
    if(n->next != NULL)
      printf(" ");
    n = n->next;
  }
  printf(")");
}

void print_indent(int indent) {
  for(int i=0; i<indent; i++)
    printf(" ");
}

int length_simple(LLIST *l) {
  int len=0;
  for(LLIST *n = l; n!=NULL; n=n->next) {
    len++;
  }
  return len;
}

int length_recur(LLIST *l) {
  int len=0;
  for(LLIST *n = l; n != NULL; n = n->next) {
    if(n->primitive) {
      len++;
    } else {
      if(n->data != NULL) {
        len+=length_recur(n->data);
      }
    }
  }
  return len;
}

void free_recur(LLIST *l) {
  printf("free_recur\n");
  LLIST *nn, *n;
  for(n=l; n != NULL; ) {
    if(n->primitive)
      free(n->data);
    else 
      if(n->data != NULL) 
        free_recur(n->data);
    nn = n->next;
    free(n);
    n=nn;
  }
}


