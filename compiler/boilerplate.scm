(define prelude "#include \"runtime.h\"\n")

(define entry-pre
  "int entry(void) {\npc=START;\nreg[continue]=END;\n")
(define entry-post "return reg[val]; \n}\n")

(define while-switch-pre
  "while(pc) {\njump: switch(pc) {\ncase START:\n")
(define while-switch-post "}\n}\n")

(define boilerplate-pre (list prelude entry-pre while-switch-pre))
(define boilerplate-post (list while-switch-post entry-post))
