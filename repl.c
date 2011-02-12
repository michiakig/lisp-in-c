#include <stdio.h>
#include <string.h> /* for strcmp */
#include "storage.h"
#include "eval.h"
#include "aux.h"
#include "read.h"
#include "types.h"
#include "env.h"

static char *in_prompt = ";;; C-eval input:";
static char *val_prompt = ";;; C-eval value:";

extern void init_symbols();

/* Top-level file, implements the REPL */
int main(int argc, char **argv) {
  object_t global_env = init_global();  /* initialize global environment */
  init_symbols();

  FILE *fp;
  struct node *input = NULL;
  object_t exp;
  object_t val;

  if(argc > 1) {
    fp = fopen(argv[1], "r");
    while(input == NULL) {
      input = read_sexp(fp);
    }
    exp = parse_sexp(input);
    val = eval(exp, &global_env);
    if(val != NULL) {
      printf("%s\n", val_prompt);
      print_object(val);
      printf("\n");
    } else
      printf("error? val was null\n");
    fclose(fp);
  }

  printf(";;; type \"q\" to quit\n");
  while(1) {
    printf("%s ", in_prompt);
    input = read_sexp(stdin);

    if(input != NULL && strcmp(input->data, "q\n") == 0)
      return 0;

    exp = parse_sexp(input);
    val = eval(exp, &global_env);

    if(val != NULL) {
      printf("%s\n", val_prompt);
      print_object(val);
      printf("\n");
    } else
      printf("error? val was null\n");

  }
}
