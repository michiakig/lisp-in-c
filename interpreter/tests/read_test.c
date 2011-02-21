#include <stdio.h>
#include <stdlib.h>
#include "../read.h"
#include "../aux.h"
#include "../storage.h"

void free_node_and_string(struct node *n) {
  free(n->data);
  free(n);
}

int main(int argc, char **argv) {
  while(1) {
    printf("> ");
    struct node *lines = read_sexp(stdin);
    object_t sexp = parse_sexp(lines);
    
    if(*(char*)lines->data == 'q') {
      auxfor_each(lines, &free_node_and_string);
      return 0;
    }
    print_object(sexp);
    printf("\n");

    auxfor_each(lines, &free_node_and_string);
  }
}
