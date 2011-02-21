#include <stdio.h>

/*
 * A simple runtime for the compiler
 */

extern void entry(void);

int main(int argc, char **argv) {
  entry();
}
