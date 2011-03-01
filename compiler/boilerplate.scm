(define prelude
  "
#include \"runtime.h\"
#include \"../interpreter/storage.h\"
#include \"../interpreter/apply.h\"
")

(define entry-pre
  "object_t entry(void) {
pc=START;
reg[continue]=label2obj(END);
init();
reg[env]=init_global();
")

(define entry-post "return reg[val]; \n}\n")

(define while-switch-pre
  "while(pc) {\njump: switch(pc) {\ncase START:\n")
(define while-switch-post "}\n}\n")

(define boilerplate-pre (list prelude entry-pre while-switch-pre))
(define boilerplate-post (list while-switch-post entry-post))
