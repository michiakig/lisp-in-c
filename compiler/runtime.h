#include "../interpreter/env.h"
#include "../interpreter/types.h"
int obj2label(object_t);
object_t label2obj(int);
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
#define TRUE 1
#define FALSE 0
#define GOTO(label) do{pc=label; goto jump;}while(0)
extern int truthy(object_t);
void save(object_t);
object_t restore();
extern int pc;
extern int flag;
extern int cont;
extern object_t reg[];

object_t lookup_variable_value(object_t, object_t);
int primitive_procedurep(object_t);
int falsep(object_t);
object_t list(object_t);

object_t compiled_procedure_env(object_t);
object_t compiled_procedure_entry(object_t);
object_t make_compiled_procedure(object_t, object_t);
object_t apply_primitive_procedure(object_t, object_t);
