
(define (surround-with-boilerplate code labels)
  (flatten (list prelude
                 (labels->defines labels)
                 entry-pre
                 while-switch-pre
                 
                 code
                 
                 while-switch-post
                 entry-post)))

(define prelude
  "#include \"runtime.h\"
#include \"storage.h\"
#include \"../interpreter/apply.h\"
")

(define entry-pre
  "object_t entry(void) {
pc=START;
reg[continue]=label2obj(END);
reg[env]=init_global();
")

(define entry-post "return reg[val]; \n}\n")

(define while-switch-pre
  "while(pc) {\njump: switch(pc) {\ncase START:\n")

(define while-switch-post "}\n}\n")

