
(define (let->lambda exp)
  (list (append (list (quote lambda)
                      (map car (cadr exp)))
                (cddr exp))
        (map cadr (cadr exp))))

(define (cond->if exp)
  )
