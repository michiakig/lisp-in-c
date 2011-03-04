
(define (if? exp) (tagged-list? 'if exp))
(define (if-predicate exp) (cadr exp))
(define (if-consequent exp) (caddr exp))
(define (if-alternative exp) (car (cdddr exp)))

(define (quoted? exp) (tagged-list? 'quote exp))
(define (text-of-quotation exp) (cadr exp))

(define (variable? exp) (tagged-list? '_user-symbol_ exp))

(define (boolean? exp) (or (eq? exp '#t) (eq? exp '#f)))
(define (self-evaluating? exp) (or (number? exp) (null? exp) (boolean? exp)))

(define (begin? exp) (tagged-list? 'begin exp))
(define (begin-actions exp) (cdr exp))

(define (tagged-list? tag lst) (and (cons? lst) (eq? tag (car lst))))

(define (assignment? exp) (tagged-list? 'set! exp))
(define (assignment-variable exp) (cadr exp))
(define (assignment-value exp) (caddr exp))

(define (definition? exp) (tagged-list? 'define exp))
(define (definition-variable exp) (cadr exp))
(define (definition-value exp) (caddr exp))

(define (lambda-params exp) (cadr exp))
(define (text-of-quotation exp) (cadr exp))
