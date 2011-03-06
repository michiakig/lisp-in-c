#include "../interpreter/storage.h"
#include "runtime.h"

int main(int argc, char **argv) {
  printf("output:");
  print_object(entry());
  printf("\n");
}
