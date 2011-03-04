
(load-with-macros "ch5-syntax.scm")

(define (assign? inst) (tagged-list? inst 'assign))
(define (goto? inst) (tagged-list? inst 'goto))
(define (test? inst) (tagged-list? inst 'test))
(define (save? inst) (tagged-list? inst 'save))
(define (restore? inst) (tagged-list? inst 'restore))
(define (perform? inst) (tagged-list? inst 'perform))

(define (label-stmt? inst) (symbol? inst))

(define (label-exp? exp) (tagged-list? exp 'label))
(define (const? exp) (tagged-list? exp 'const))
(define (reg? exp) (tagged-list? exp 'reg))

(define (make-string x)
  (cond ((string? x) x)
        ((symbol? x) (symbol->string x))
        ((number? x) (number->string x))
        (else "?")))

(define (stmt->c inst)
  (cond ((assign? inst) (assign->c inst))
        ((label-stmt? inst) (label->c inst))
        ((goto? inst) (goto->c inst))
        (else (list "???"))
        ))

(define (exp->c exp)
  (cond ((label-exp? exp) (label->c exp))
        ((reg? exp) (reg->c exp))
        ((const? exp) (const->c exp))
        ))

(define (goto->c inst)
  (list "GOTO(" (exp->c (cadr inst)) ")"))

(define (label->c inst)
  (if (symbol? inst)
      (list "case " inst ":\n")
      (list "label2obj(" (cadr inst) ")")))

(define (assign->c inst)
  (if (tagged-list? (caddr inst) 'op)
      (append (list (reg->c (cadr inst)) "=")
              (opcall->c (caddr inst) (cdddr inst)))
      (append (list (reg->c (cadr inst)) "=")
              (exp->c (caddr inst)))))

(define (opcall->c op args)
  (append (list (cadr op) "(")
          (interleave "," args)
          (list ")")))

(define (reg->c name) (list "reg[" name "]"))

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
