
(define (surround-with-boilerplate code labels)
  (flatten (list prelude
                 (labels->defines labels)
                 entry-pre
                 while-switch-pre
                 
                 code
                 
                 while-switch-post
                 entry-post)))

(define prelude
  "#include \quoteruntime.h\quote\newline")

(define entry-pre
  "object_t entry(void) {\newlinepc=START;\newlinereg[continue]=label2obj(END);\newline\newlinereg[env]=init_global();\newline")

(define entry-post "return reg[val]; \newline}\newline")

(define while-switch-pre
  "while(pc) {\newlinejump: switch(pc) {\newlinecase START:\newline")

(define while-switch-post "}\newline}\newline")

