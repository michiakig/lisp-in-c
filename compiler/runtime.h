#include "env.h"
#include "../interpreter/types.h"
#include "../interpreter/apply.h"
#include "../interpreter/primitives.h"
#include "storage.h"
int obj2label(object_t);
object_t label2obj(int);
#define START 1
#define END 0
#define NUM_REGISTERS 10
#define exp 0
#define env 1
#define val 2
#define continue 3
#define proc 4
#define argl 5
#define unev 6
#define gen1 7 /* three "general purpose" registers */
#define gen2 8
#define gen3 9

#define TRUE 1
#define FALSE 0
#define GOTO(label) do{pc=label; goto jump;}while(0)
extern int truthy(object_t);

extern int pc;
extern int flag;
extern int cont;

object_t lookup_variable_value(object_t, object_t);
int primitive_procedurep(object_t);
int falsep(object_t);
object_t list(object_t);

object_t compiled_procedure_env(object_t);
object_t compiled_procedure_entry(object_t);
object_t make_compiled_procedure(object_t, object_t);
object_t apply_primitive_procedure(object_t, object_t);

extern object_t entry();
