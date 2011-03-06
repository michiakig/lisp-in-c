#include <stdio.h>
#include "storage.h"
#include "runtime.h"

int main(int argc, char **argv) {
  init_heap();
  printf("output:");
  print_object(entry());
  printf("\n");
}
