
(load-with-macros "compiler.scm")

(compile-and-emit '(apply + (list 1 2 3))
                  "file.c")
