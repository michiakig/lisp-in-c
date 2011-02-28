
(define (interleave x lst)
  (if (or (null? lst) (null? (cdr lst)))
      lst
      (cons (car lst)
            (cons x
                  (interleave x (cdr lst))))))

;; from SICP:
(define (list-union s1 s2)
  (cond ((null? s1) s2)
        ((memq (car s1) s2) (list-union (cdr s1) s2))
        (else (cons (car s1) (list-union (cdr s1) s2)))))

(define (list-difference s1 s2)
  (cond ((null? s1) ())
        ((memq (car s1) s2) (list-difference (cdr s1) s2))
        (else (cons (car s1)
                    (list-difference (cdr s1) s2)))))
