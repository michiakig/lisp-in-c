
(define (_expandquasi_ exp)
  (define (_expandquasir_ exp)
    (if (symbol? exp)
        (list 'quote exp)
        (cons 'list 
              (map (lambda (e)
                     (if (cons? e)
                         (if (eq? (car e) 'unquote)
                             (cadr e)
                             (_expandquasir_ e))
                         (list 'quote e)))
                   exp))))
  (_expandquasir_ (cadr exp)))


(define (_expandand_ exp)
  (define (_expandandr_ operands)
    (if (null? operands)
        '#t
        (list 'if (car operands)
              (_expandandr_ (cdr operands))
              '#f)))
  (_expandandr_ (cdr exp)))

(define (_expandor_ exp)
  (define (_expandorr_ operands)
    (if (null? operands)
        '#f
        (list 'if (car operands)
              '#t
              (_expandorr_ (cdr operands)))))
  (_expandorr_ (cdr exp)))

(define (_expandlet_ exp)
  (cons (append (list (quote lambda)
                       (map car (cadr exp)))
                 (cddr exp))
        (map cadr (cadr exp))))

(define (cond-clauses exp)
  (cdr exp))
(define (first-clause exp)
  (car exp))
(define (rest-clauses exp)
  (cdr exp))
(define (clause-pred clause)
  (car clause))
(define (clause-consq clause)
  (cons (quote begin) (cdr clause)))

(define (_expandcond_ exp)
  (define (_expandcond_r_ clauses)
    (if (null? (rest-clauses clauses))
        (if (eq? (clause-pred (first-clause clauses)) (quote else))
            (clause-consq (first-clause clauses))
            (list (quote if)
                  (clause-pred (first-clause clauses))
                  (clause-consq (first-clause clauses))))
        (list (quote if)
              (clause-pred (first-clause clauses))
              (clause-consq (first-clause clauses))
              (_expandcond_r_ (rest-clauses clauses)))))
  (_expandcond_r_ (cond-clauses exp)))

(define _macros_ (list
                  (cons 'let _expandlet_)
                  (cons 'cond _expandcond_)
                  (cons 'and _expandand_)
                  (cons 'or _expandor_)
                  (cons 'quasiquote _expandquasi_)))

(define _identity_ (lambda (x) x))

(define (_macro?_ exp)
  (if (assoc (car exp) _macros_)
      (cdr (assoc (car exp) _macros_))
      _identity_))

(define (_expand_ exp)
  (if (null? exp)
      ()
      (if (cons? exp)
          (if (not (eq? (car exp) 'quote))
              (begin
                (set! exp ((_macro?_ exp) exp))
                (cons (_expand_ (car exp))
                      (improper-map _expand_ (cdr exp))))
              exp)
          exp)))

(define (load-with-macros filename)
  (eval (_expand_ (read-file filename))))
