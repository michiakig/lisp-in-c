
(load-with-macros "compiler.scm")

(compile-and-emit '(begin
                     (define (factorial n)
                       (if (= n 1)
                           1
                           (* (factorial (- n 1)) n)))
                     (factorial 5))
                  "file.c")
