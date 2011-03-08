

(load-with-macros "compiler.scm")

(compile-and-emit '(begin
                     (define (sum n)
                       (if (= n 0)
                           0
                           (+ n (sum (- n 1)))))
                     (sum 5))
                  "file.c")
