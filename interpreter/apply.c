#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "eval.h"
#include "apply.h"

#include "storage.h"
#include "env.h"
#include "obarray.h"
#include "types.h"
#include "reader.h"
#include "primitives.h"
#include "lib/str_utils.h"

object_t init_global() {
  object_t env = NIL;
  object_t primitives[] = {
    obj_new_symbol("+"),         obj_new_primitive(&primitive_add),
    obj_new_symbol("*"),         obj_new_primitive(&primitive_multiply),
    obj_new_symbol("-"),         obj_new_primitive(&primitive_subtract),
    obj_new_symbol("/"),         obj_new_primitive(&primitive_divide),
    obj_new_symbol("<"),         obj_new_primitive(&primitive_lessthan),

    obj_new_symbol(">"),         obj_new_primitive(&primitive_greaterthan),
    obj_new_symbol("="),         obj_new_primitive(&primitive_equals),
    obj_new_symbol("cons"),      obj_new_primitive(&primitive_cons),
    obj_new_symbol("car"),       obj_new_primitive(&primitive_car),
    obj_new_symbol("cdr"),       obj_new_primitive(&primitive_cdr),

    obj_new_symbol("null?"),     obj_new_primitive(&primitive_isnull),
    obj_new_symbol("eq?"),       obj_new_primitive(&primitive_eq),
    obj_new_symbol("set-cdr!"),  obj_new_primitive(&primitive_set_cdr),
    obj_new_symbol("set-car!"),  obj_new_primitive(&primitive_set_car),
    obj_new_symbol("symbol?"),   obj_new_primitive(&primitive_symbolp),

    obj_new_symbol("cons?"),     obj_new_primitive(&primitive_consp),
    obj_new_symbol("load"),      obj_new_primitive(&primitive_load),
    obj_new_symbol("print"),     obj_new_primitive(&primitive_print),
    obj_new_symbol("eval"),      obj_new_primitive(&primitive_eval),
    obj_new_symbol("apply"),     obj_new_primitive(&primitive_apply),

        obj_new_symbol("read"),      obj_new_primitive(&primitive_read),
    obj_new_symbol("read-file"), obj_new_primitive(&primitive_read_file),
    obj_new_symbol("quit"),      obj_new_primitive(&primitive_quit),
    obj_new_symbol("error"),     obj_new_primitive(&primitive_error),
    obj_new_symbol("string?"),   obj_new_primitive(&primitive_stringp),

    obj_new_symbol("number?"),   obj_new_primitive(&primitive_numberp),
    obj_new_symbol("file-append"),    obj_new_primitive(&primitive_file_append),
    obj_new_symbol("symbol->string"), obj_new_primitive(&primitive_symbol2string),
    obj_new_symbol("string->symbol"), obj_new_primitive(&primitive_string2symbol),
    obj_new_symbol("string-append"), obj_new_primitive(&primitive_string_append),

    obj_new_symbol("number->string"), obj_new_primitive(&primitive_number2string),
    obj_new_symbol("read-char"),    obj_new_primitive(&primitive_read_char),
    obj_new_symbol("string-length"),    obj_new_primitive(&primitive_string_length),
    obj_new_symbol("string="),    obj_new_primitive(&primitive_string_equals),
    obj_new_symbol("string-ref"),    obj_new_primitive(&primitive_string_ref)
  };

  int i;
  for(i = 0; i < 70; i+=2)
    env = define_variable(primitives[i], primitives[i+1], env);

  return env;
}

struct proc_t {
  object_t (*fn) (object_t);
  object_t params;
  object_t body;
  object_t env;
};

int isprimitiveproc(proc_t p) {
  return p->fn != NULL;
}

int iscompoundproc(proc_t p) {
  return p->fn == NULL;
}

extern object_t eval_sequence(object_t, object_t *);

object_t lambda_env(object_t p) {
  proc_t proc = obj_get_proc(p);
  return proc->env;
}

object_t lambda_params(object_t p) {
  proc_t proc = obj_get_proc(p);
  return proc->params;
}

object_t lambda_body(object_t p) {
  proc_t proc = obj_get_proc(p);
  return proc->body;  
}

object_t obj_new_compound(object_t params, object_t body, object_t env ) {
  proc_t proc = malloc(sizeof(*proc));
  proc->params = params;
  proc->body = body;
  proc->env = env;
  return obj_new_proc(proc);
}

object_t obj_new_primitive(object_t (*fn) (object_t)) {
  proc_t proc = malloc(sizeof(*proc));
  proc->fn = fn;
  return obj_new_proc(proc);
}

object_t apply(object_t p, object_t argl) {
  proc_t proc = obj_get_proc(p);
  if(isprimitiveproc(proc)) /* primitive procedure */
    return (proc->fn)(argl);
  else {
    object_t extended = extend_environment(lambda_params(p), argl, lambda_env(p));
    return eval_sequence(lambda_body(p), &extended);
  }
}
