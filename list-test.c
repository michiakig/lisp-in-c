#include <stdlib.h>
#include <stdio.h>

#include "list.h"

int main(int argc, char **argv) {
  LIST *head = malloc(sizeof(LIST));
  head->next = NULL;
  head->type = ATOM;
  head->data = argv[1];
  LIST *node = NULL;

  for(int i=2; i<4; i++) {
    node = malloc(sizeof(LIST));
    node->data = argv[i];
    node->type = ATOM;
    node->next = NULL;
    head = cons(head, node);
  }
  print_boxp(head);
  printf(" %d\n", length(head));

  LIST *nested = malloc(sizeof(LIST));
  nested->next = NULL;
  nested->type = ATOM;
  nested->data = argv[4];

  for(int i=5; i<7; i++) {
    node = malloc(sizeof(LIST));
    node->data = argv[i];
    node->type = ATOM;
    node->next = NULL;
    nested = cons(nested, node);
  }


  LIST *n = malloc(sizeof(LIST));
  n->next = NULL;
  n->type = CONS;
  n->data = nested;

  head = cons(head, n);

  for(int i=7; i<8; i++) {
    node = malloc(sizeof(LIST));
    node->data = argv[i];
    node->type = ATOM;
    node->next = NULL;
    head = cons(head, node);
  }

  print_boxp(head);
  printf(" %d\n", length_r(head));

  free_r(head);

//  for(int i=1; i<argc; i++) {
//    node = malloc(sizeof(LIST));
//    node->data = argv[i];
//    node->type = ATOM;
//    node->next = NULL;

//  }
//  print(head);
//  printf("\n");
//
//  for(int i=1; i<argc; i++) {
//    node = pop(&head);
//    printf("%s\n", (char*)node->data);
//    print(head);
//    printf("\n");
//  }
}
