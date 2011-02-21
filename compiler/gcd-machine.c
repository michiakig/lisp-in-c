
/*
 * Testing my concept for translating the SICP register machine
 * language to C -- the gcd machine at the beginning of section 5.1
 */

#include <stdio.h>

int pc;
int t;
int a;
int b;

void entry(void) {
  pc = 1;
  a = 104;
  b = 32;
  while(pc)
    switch(pc) {
    case 1: /* test-b */
      if(b == 0) {
        pc = 2;
        break;
      }
      t = a % b;
      a = b;
      b = t;
      break;
    case 2: /* gcd-done*/
      pc = 0;
      break; 
    }
  printf("%d\n", a);
}
