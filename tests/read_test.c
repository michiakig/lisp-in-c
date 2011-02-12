#include <stdio.h>
#include <stdlib.h>
#include "../hasht.h"
#include "../read.h"
#include "../aux.h"

void free_node_and_string(struct node *n) {
  free(n->data);
  free(n);
}

struct nlist *obarray[100];

int main(int argc, char **argv) {
  while(1) {
    printf("> ");
    struct node *lines = read_sexp(stdin);
    list *sexp = parse_sexp(lines, obarray);
    
    if(*(char*)lines->data == 'q') {
      auxfor_each(lines, &free_node_and_string);
      exit(0);
    }
    print_boxp(sexp);
    printf("\n");

    auxfor_each(lines, &free_node_and_string);
  }
}
