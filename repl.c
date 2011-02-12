#include <stdio.h>
#include <string.h> /* for strcmp */
#include "storage.h"
#include "eval.h"
#include "aux.h"
#include "read.h"

static char *in_prompt = ";;; C-eval input:";
static char *val_prompt = ";;; C-eval value:";

extern void init_symbols();

/* Top-level file, implements the REPL */
int main(int argc, char **argv) {
  /* initialize global environment */
  object_t global_env;
  init_symbols();
  global_env = NIL; /*init_global(hashtable); */

  if(argc > 1) {
    /* TODO: open the source file and process it*/
  } /* otherwise start the REPL */

  printf(";;; type \"q\" to quit\n");
  struct node *input = NULL;
  while(1) {
    printf("%s ", in_prompt);
    input = read_sexp(stdin);

    if(input != NULL && strcmp(input->data, "q\n") == 0) {
      return 0;
    }

    object_t exp = parse_sexp(input);
    object_t val = eval(exp, &global_env);

    if(val != NULL) {
      printf("%s\n", val_prompt);
      print_object(val);
      printf("\n");
    } else {
      printf("error? val was null\n");
    }
  }
  /*  simple_rfree(global_env);
      free_hasht(hashtable); */
}
