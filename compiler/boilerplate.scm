(define prelude
  "
#include \"runtime.h\"
#include \"../interpreter/storage.h\"
")

(define entry-pre
  "object_t entry(void) {\npc=START;\nreg[continue]=label2obj(END);\ninit();\n")

(define entry-post "return reg[val]; \n}\n")

(define while-switch-pre
  "while(pc) {\njump: switch(pc) {\ncase START:\n")
(define while-switch-post "}\n}\n")

(define boilerplate-pre (list prelude entry-pre while-switch-pre))
(define boilerplate-post (list while-switch-post entry-post))
