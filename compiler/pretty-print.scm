
;; pretty-print register machine code (or generated C code)
(define (pretty-print-c printfn lst)
  (for-each (lambda (x)
              (printfn x))
            lst))

;; pretty-print register machine code
(define (pretty-print-reg code)
  (for-each (lambda (x)
              (if (symbol? x)
                  (print x)
                  (begin
                    (print "  ")
                    (print x)))
              (print "\n"))
            code))

