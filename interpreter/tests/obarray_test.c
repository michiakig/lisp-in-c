#include <stdio.h>
#include "../obarray.h"

int main (int argc, char **argv) {
  if (argc < 2) {
    printf("too few args\n");
    return 1;
  }

  int i;
  for (i = 1; i < argc; i++ ) {
    symbol_t sym = intern(argv[i]);
    printf("interned: %s\n", name(sym));
  }
}
