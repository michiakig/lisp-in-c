;; code for emitting C to a file

(define (emit stmts f)
  (for-each (lambda (s) (file-append s f)) boilerplate-pre)
  (emit-statement stmts f)
  (for-each (lambda (s) (file-append s f)) boilerplate-post))

(define (emit-register reg filename)
  (file-append "reg[" filename)
  (file-append (symbol->string reg) filename)
  (file-append "]" filename))

(define (emit-constant const filename)
  (cond ((number? const)
         (file-append (symbol->string const) filename))
        (error "ERROR constant not a number")))

(define (emit-statement statement filename)
  (define (emit_r parts)
    (cond ((null? parts) 'done)
          ((string? (car parts)) (file-append (car parts) filename))
          ((cons? (car parts))
           (cond ((eq? 'reg (caar parts))
                  (emit-register (cadar parts) filename))
                 ((eq? 'const (caar parts))
                  (emit-constant (cadar parts) filename))))
          (else 'ERROR))
    (emit_r (cdr parts)))
  (emit_r statement))
