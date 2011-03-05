
;; pretty-print register machine code (or generated C code)
(define (pretty-print-c lst)
  (for-each (lambda (x)
              (print "  ")
              (print x))
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

