
(define compiled (compile
                   '(define (factorial n)
                      (if (= n 1)
                          1
                          (* (factorial (- n 1)) n)))
                   'val
                   'next))

;; pretty-print register machine code
(define (pprint lst)
  (newline)
  (for-each (lambda (x)
              (if (symbol? x)
                  (display x)
                  (begin
                    (display "  ")
                    (display x)))
              (newline))
            lst))

(pprint (caddr compiled))
