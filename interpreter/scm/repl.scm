(load "/Users/aki/lisp-in-c/interpreter/scm/lib.scm")
(load "/Users/aki/lisp-in-c/interpreter/scm/macros.scm")

(load-with-macros "/Users/aki/lisp-in-c/interpreter/scm/extlib.scm" )

(define (_repl_)
  (define _exp_ ())
  (print "> ")
  (set! _exp_ (read))
  (if (eq? _exp_ (quote quit))
      (quit)
      (if (not (eq? _exp_ (quote _empty_)))
          (begin
            (print (eval (_expand_ _exp_)))
            (print "\newline")
            (_repl_))
          (_repl_))))
(_repl_)
