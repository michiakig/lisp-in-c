
(load-with-macros "register2c.scm")
(load-with-macros "pprint.scm")

(define (run-visual-test exp)
  (print "\n-- ")
  (print exp)
  (print "\n\n")
  (pretty-print-reg (caddr (compile exp 'val 'return)))
  (print "\n")
  (pretty-print-c (lisp->c exp)))


(run-visual-test '1)

(run-visual-test 'foo)

(run-visual-test '(define foo 1))

(run-visual-test '(begin (define foo 1) foo))

(run-visual-test '(begin (+ 1 1)))







