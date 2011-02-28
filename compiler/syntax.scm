
(define (variable? exp) (tagged-list? '_user-symbol_ exp))

(define (self-evaluating? exp) (number? exp))
(define (begin? exp) (tagged-list? 'begin exp))
(define (begin-actions exp) (cdr exp))

(define (tagged-list? tag lst) (and (cons? lst) (eq? tag (car lst))))

(define (definition? exp) (tagged-list? 'define exp))
(define (definition-variable exp) (cadr exp))
(define (definition-value exp) (caddr exp))

(define (lambda-params exp) (cadr exp))
(define (text-of-quotation exp) (cadr exp))
