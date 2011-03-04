
(define (assign? inst) (tagged-list? 'assign inst))
(define (goto? inst) (tagged-list? 'goto inst))
(define (test? inst) (tagged-list? 'test inst))
(define (save? inst) (tagged-list? 'save inst))
(define (restore? inst) (tagged-list? 'restore inst))

(define (label? exp) (tagged-list? 'label exp))
(define (const? exp) (tagged-list? 'const exp))
(define (reg? exp) (tagged-list? 'reg exp))

(define (make-string x)
  (cond ((string? x) x)
        ((symbol? x) (symbol->string x))
        ((number? x) (number->string x))
        (else "?")))

(define (assign->c inst)
  (if (tagged-list? 'op (caddr inst))
      (append (list (register->c (cadr inst)) "=")
              )))

(define (register->c name) (list "reg[" name "]"))

(define (const->c const)
  (cond ((number? const) const)
        ((symbol? const) (list "obj_new_symbol(" const ")"))
        ((string? const) (list "obj_new_string(\"" const "\")"))
        ((cons? const)
         (list->cons-calls const))))

; (a b c) --> "cons(a, cons(b, cons(c, NIL)))"
(define (list->cons-calls lst)
  (if (null? (cdr lst))
      (list "cons(" (car lst) ",NIL)")
      (append (list "cons(" (car lst) ",")
              (list->cons-calls (cdr lst))
              (list ")"))))

