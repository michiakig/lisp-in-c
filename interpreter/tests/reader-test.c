#include <stdio.h>
#include "../runtime/reader.h"
#include "../storage.h"

int main(int argc, char **argv) {

  while(1) {
    object_t exp = read_stream(stdin);
    printf("--\n");
    print_object(exp);
    if(iscons(exp) && isproperlist(exp))
      printf(" proper list");
    else
      printf(" symbol or improper list");
    printf("\n");
  }

}
