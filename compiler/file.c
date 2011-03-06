#include "runtime.h"
#include "storage.h"
#include "../interpreter/apply.h"
#define after_call20 21
#define primitive_branch18 20
#define compile_branch19 19
#define after_lambda2 18
#define after_if5 17
#define after_call17 16
#define primitive_branch15 15
#define compile_branch16 14
#define after_call14 13
#define primitive_branch12 12
#define compile_branch13 11
#define after_call11 10
#define primitive_branch9 9
#define compile_branch10 8
#define false_branch4 7
#define true_branch3 6
#define after_call8 5
#define primitive_branch6 4
#define compile_branch7 3
#define entry1 2
object_t entry(void) {
pc=START;
reg[continue]=label2obj(END);
reg[env]=init_global();
while(pc) {
jump: switch(pc) {
case START:
reg[val]=make_compiled_procedure(label2obj(entry1),reg[env]);
GOTO(after_lambda2);
case entry1:;
reg[env]=compiled_procedure_env(reg[proc]);
reg[env]=extend_environment(cons(obj_new_symbol("n"),NIL),reg[argl],reg[env]);
save(reg[continue]);
save(reg[env]);
reg[proc]=lookup_variable_value(obj_new_symbol("="),reg[env]);
reg[val]=obj_new_number(1);
reg[argl]=list(reg[val]);
reg[val]=lookup_variable_value(obj_new_symbol("n"),reg[env]);
reg[argl]=cons(reg[val],reg[argl]);
flag=primitive_procedurep(reg[proc]);
if(flag){GOTO(primitive_branch6);}
case compile_branch7:;
reg[continue]=label2obj(after_call8);
reg[val]=compiled_procedure_entry(reg[proc]);
GOTO(obj2label(reg[val]));
case primitive_branch6:;
reg[val]=apply_primitive_procedure(reg[proc],reg[argl]);
case after_call8:;
reg[env]=restore();
reg[continue]=restore();
flag=falsep(reg[val]);
if(flag){GOTO(false_branch4);}
case true_branch3:;
reg[val]=obj_new_number(1);
GOTO(obj2label(reg[continue]));
case false_branch4:;
reg[proc]=lookup_variable_value(obj_new_symbol("*"),reg[env]);
save(reg[continue]);
save(reg[proc]);
reg[val]=lookup_variable_value(obj_new_symbol("n"),reg[env]);
reg[argl]=list(reg[val]);
save(reg[argl]);
reg[proc]=lookup_variable_value(obj_new_symbol("factorial"),reg[env]);
save(reg[proc]);
reg[proc]=lookup_variable_value(obj_new_symbol("-"),reg[env]);
reg[val]=obj_new_number(1);
reg[argl]=list(reg[val]);
reg[val]=lookup_variable_value(obj_new_symbol("n"),reg[env]);
reg[argl]=cons(reg[val],reg[argl]);
flag=primitive_procedurep(reg[proc]);
if(flag){GOTO(primitive_branch9);}
case compile_branch10:;
reg[continue]=label2obj(after_call11);
reg[val]=compiled_procedure_entry(reg[proc]);
GOTO(obj2label(reg[val]));
case primitive_branch9:;
reg[val]=apply_primitive_procedure(reg[proc],reg[argl]);
case after_call11:;
reg[argl]=list(reg[val]);
reg[proc]=restore();
flag=primitive_procedurep(reg[proc]);
if(flag){GOTO(primitive_branch12);}
case compile_branch13:;
reg[continue]=label2obj(after_call14);
reg[val]=compiled_procedure_entry(reg[proc]);
GOTO(obj2label(reg[val]));
case primitive_branch12:;
reg[val]=apply_primitive_procedure(reg[proc],reg[argl]);
case after_call14:;
reg[argl]=restore();
reg[argl]=cons(reg[val],reg[argl]);
reg[proc]=restore();
reg[continue]=restore();
flag=primitive_procedurep(reg[proc]);
if(flag){GOTO(primitive_branch15);}
case compile_branch16:;
reg[val]=compiled_procedure_entry(reg[proc]);
GOTO(obj2label(reg[val]));
case primitive_branch15:;
reg[val]=apply_primitive_procedure(reg[proc],reg[argl]);
GOTO(obj2label(reg[continue]));
case after_call17:;
case after_if5:;
case after_lambda2:;
define_variable(obj_new_symbol("factorial"),reg[val],reg[env]);
reg[val]=obj_new_symbol("ok");
reg[proc]=lookup_variable_value(obj_new_symbol("factorial"),reg[env]);
reg[val]=obj_new_number(5);
reg[argl]=list(reg[val]);
flag=primitive_procedurep(reg[proc]);
if(flag){GOTO(primitive_branch18);}
case compile_branch19:;
reg[val]=compiled_procedure_entry(reg[proc]);
GOTO(obj2label(reg[val]));
case primitive_branch18:;
reg[val]=apply_primitive_procedure(reg[proc],reg[argl]);
GOTO(obj2label(reg[continue]));
case after_call20:;
}
}
return reg[val]; 
}
