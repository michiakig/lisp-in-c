#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for strcmp */
#include "storage.h"
#include "eval.h"
#include "apply.h"
#include "runtime/reader.h"
#include "types.h"
#include "env.h"

object_t global_env;

extern void init_symbols();

object_t load_file(char *filename, object_t *env) {
  object_t exp = read_file(filename);
  return eval(exp, env);
}

/* Top-level file, implements the REPL 
int main(int argc, char **argv) {
  init_heap();
  global_env = init_global();
  init_symbols();

  if(argc > 1) {
    int i;
    for(i = 1; i<argc; i++) {
      load_file(argv[i], &global_env);
      printf("loaded %s.\n", argv[i]);
    }
  }
}
*/
