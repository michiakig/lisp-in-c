;; code for emitting C to a file

(load "boilerplate.scm")

(define (emit-user-symbols symbols f)
  (if (= 0 (length symbols))
      'done
      (begin
        (file-append "object_t symbols[" f)
        (file-append (symbol->string (length symbols)) f)
        (file-append "];\n" f)
        (file-append "void init(void){\n" f)
        (file-append "int s=0;\n" f)
        (for-each (lambda (s)
                    (file-append "symbols[s++]=obj_new_symbol(\"" f)
                    (file-append (symbol->string s) f)
                    (file-append "\");\n" f))
                  symbols)
        (file-append "}\n" f)
        'done)))

(define (emit stmts f)
  (file-append prelude f)
  (emit-user-symbols user-symbols f)
  (file-append entry-pre f)
  (file-append while-switch-pre f)
  (emit-statement stmts f)
  (for-each (lambda (s) (file-append s f)) boilerplate-post))

(define (emit-register reg filename)
  (file-append "reg[" filename)
  (file-append (symbol->string reg) filename)
  (file-append "]" filename))

(define (emit-constant const filename)
  (cond ((number? const)
         (file-append "obj_new_symbol(\"" filename)
         (file-append (symbol->string const) filename)
         (file-append "\")" filename))

        (error "ERROR constant not a number")))

(define (emit-user-symbol index filename)
  (file-append "symbols[" filename)
  (file-append (symbol->string index) filename)
  (file-append "]" filename))

(define (emit-statement statement filename)
  (define (emit-r parts)
    (cond ((null? parts) 'done)
          ((string? (car parts)) (file-append (car parts) filename))
          ((cons? (car parts))
           (cond ((eq? 'reg (caar parts))
                  (emit-register (cadar parts) filename))
                 ((eq? 'const (caar parts))
                  (emit-constant (cadar parts) filename))
                 ((eq? '_user-symbol_ (caar parts))
                  (emit-user-symbol (cadar parts) filename))
                 (else ; expression
                  (emit-r (car parts)))))
          (else 'ERROR))
    (emit-r (cdr parts)))
  (emit-r statement))
