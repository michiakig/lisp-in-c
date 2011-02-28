


(define (collect exp)
  (cond ((null? exp) ())
        ((and (not (cons? exp))
              (not (self-evaluating? exp))) (list exp))
        ((self-evaluating? exp) ())
        ((definition? exp)
         (list (definition-variable exp)))
        ((eq? (car exp) 'lambda)
         (lambda-params exp))
        ((eq? (car exp) 'quote)
         (if (cons? (text-of-quotation exp))
             (text-of-quotation exp)
             (list (text-of-quotation exp))))
        (else ())))

(define (make-user-symbol-placeholder sym)
  (list '_user-symbol_ (position sym user-symbols)))

(define (replace-user-symbols exp)
  (cond ((null? exp) ())
        ((and (not (cons? exp))
              (not (self-evaluating? exp)))
         (make-user-symbol-placeholder exp))
        ((definition? exp)
         (list 'define
               (make-user-symbol-placeholder (definition-variable exp))
               (definition-value exp)))
        (else exp)))
