

(load-with-macros "compiler.scm")

(compile-and-emit '(begin
                     (define foo 1)
                     (+ foo foo))
                  "file.c")
