#define STACKSIZE 100
#define START 1
#define END 0
#define exp 0
#define env 1
#define val 2
#define continue 3
#define proc 4
#define argl 5
#define unev 6
#define GOTO(label) do{pc=label; goto jump;}while(0)
void push(int);
int pop();
extern int pc;
extern int reg[];
