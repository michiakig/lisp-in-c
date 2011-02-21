#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for strcmp */
#include "storage.h"
#include "eval.h"
#include "apply.h"
#include "aux.h"
#include "read.h"
#include "types.h"
#include "env.h"

static char *welcome = ";;; type \"q\" to quit";
static char *in_prompt = ";;; C-eval input:";
static char *val_prompt = ";;; C-eval value:";

object_t global_env;

extern void init_symbols();

void print_sequence(object_t seq) {
  while(!nilp(seq)) {
    print_object(car(seq));
    printf("\n");
    seq = cdr(seq);
  }
}

object_t load_file(char *filename, object_t *env) {
  FILE *fp = fopen(filename, "r");
  struct node *input = read_sexp(fp);
  object_t val, exp, ret = NIL;
  while(input != NULL) {
    if(input->data == NULL) {
      free(input);
      input = read_sexp(fp);
      continue;
    }
    exp = parse_sexp(input);
    if(exp!=NULL) {
      val = eval(exp, env);
      if(val != NULL)
        if(nilp(ret))
          ret = cons(val, NIL);
        else
          storage_append(val, ret);
    }
    input = read_sexp(fp);
  }
  fclose(fp);
  return ret;
}

/* Top-level file, implements the REPL */
int main(int argc, char **argv) {
  init_heap();
  global_env = init_global();  /* initialize global environment */
  init_symbols();
  struct node *input = NULL;
  object_t exp;
  object_t val;

  if(argc > 1) {
    int i;
    for(i = 1; i<argc; i++) {
      object_t val = load_file(argv[i], &global_env);
      print_sequence(val);
      printf("loaded %s.\n", argv[i]);
    }
  }

  printf("%s\n", welcome);
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
    }
  }
}
