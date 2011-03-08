
(load-with-macros "compiler.scm")

(compile-and-emit '(begin
                     (let ((foo 1)
                           (bar 2))
                       (+ foo bar)))
                  "file.c")
