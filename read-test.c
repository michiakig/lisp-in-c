#include <stdio.h>

#include "list.h"
#include "read.h"
#include "str_utils.h"

void remove_newlines_list(LIST *n) {
  if(n != NULL && n->type == ATOM)
    remove_newline((char*)n->data);
}

int main(int argc, char **argv) {
  LIST *sexp;
  while(1) {
    printf("> ");
    sexp = parse_sexp(read_sexp(stdin));
    if(sexp->type == CONS) {
      print_boxp((LIST*)sexp->data);
      printf("\n");
      print_sexp((LIST*)sexp->data);
      printf("\n");
    } else if(sexp->type == ATOM) {
      printf("%s", (char*)sexp->data);
      printf("\n");
    }
  }
}
