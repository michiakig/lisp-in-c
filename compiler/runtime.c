/* A simple runtime for the compiler */
#include <stdio.h>
#include "runtime.h"
int pc;
int reg[7] = {0};
int stack[STACKSIZE] = {0};
int *sp = stack;
void push(int x) {
  *sp = x; sp++;
}
int pop() {
  sp--; return *sp;
}

extern int entry(void);

int main(int argc, char **argv) {
  printf("output: %d\n", entry());
}
