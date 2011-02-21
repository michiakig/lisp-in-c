#include <stdio.h>

#define STACKSIZE 100
int stack[STACKSIZE] = {0};
int *sp = stack;

void save(int x) { *sp = x; sp++; }
int restore() { sp--; return *sp; }

#define GOTO(label) do{pc=label; goto jump;}while(0)

int pc;
int continue_;
int n, val;

#define END 0
#define FACT_LOOP 1
#define AFTER_FACT 2
#define BASE_CASE 3
#define FACT_DONE 4

void entry(void) {
  continue_ = FACT_DONE;
  pc = FACT_LOOP;
  n = 5;

  while(pc) {
  jump: switch(pc) {

    case FACT_LOOP:
      if(n == 1)
        GOTO(BASE_CASE);

      save(continue_);
      save(n);
      n = n - 1;
      continue_ = AFTER_FACT;
      GOTO(FACT_LOOP);

    case AFTER_FACT:
      n = restore();
      continue_ = restore();
      val = n * val;
      GOTO(continue_);

    case BASE_CASE:
      val = 1;
      GOTO(continue_);

    case FACT_DONE:
      GOTO(END);

    }
  }
  printf("%d\n", val);
}
