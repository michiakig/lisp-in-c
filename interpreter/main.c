#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for strcmp */

#include "eval.h"
#include "apply.h"

#include "storage.h"
#include "reader.h"
#include "types.h"
#include "env.h"

extern object_t global_env;

extern void init_symbols();

int main(int argc, char **argv) {
  init_heap();
  global_env = init_global();
  init_symbols();
  if(argc > 1) {
    int i;
    for(i = 1; i<argc; i++)
      load_file(argv[i], &global_env);
  }
}

