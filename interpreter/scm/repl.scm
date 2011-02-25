
(define _exp_ ())

(define (_repl_)
  (print "> ")
  (set! _exp_ (read))
  (if (eq? _exp_ (quote quit))
      (quit)
      (begin
        (print (eval _exp_))
        (print "\n")
        (_repl_))
      ;; (if (not (eq? _exp_ (quote _empty_)))
      ;;     (begin
      ;;       (print (eval (_expand_ _exp_)))
      ;;       (print "\n")
      ;;       (_repl_))
      ;;     (_repl_))
      ))

(_repl_)
