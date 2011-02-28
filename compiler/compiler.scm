;; The compiler

(load-with-macros "syntax.scm")
(load-with-macros "collect.scm")
(load-with-macros "emit.scm")

(define user-symbols ())

(define (compile exp target linkage)
  (set! user-symbols (append user-symbols (list-difference (collect exp) user-symbols)))
  (set! exp (replace-user-symbols exp))
  (print exp)
  (print "\n")
  (cond ((self-evaluating? exp)
         (compile-self-evaluating exp target linkage))
        ((definition? exp)
         (compile-definition exp target linkage))
        ((variable? exp)
         (compile-variable exp target linkage))
        ((begin? exp)
         (compile-sequence (begin-actions exp) target linkage))
        (else
         (error "Unknown expression type -- COMPILE" exp))))

;; statements
(define (make-goto-statement location)
 (list "GOTO(" location ");\n"))
(define (make-assignment-statement target value)
  (list target "=" value ";\n"))
(define (make-save-statement reg)
 (list "save(" reg ");\n"))
(define (make-restore-statement reg)
  (list reg "=restore();\n"))

(define (make-perform-statement op . args)
  (append  (append (list op "(")
                   (interleave "," args))
           (list ");\n")))

;; expressions
(define (make-lookup-expression var env)
  (list "lookup_variable(" var "," env ")"))

(define (make-instruction-sequence needs modifies statements)
  (list needs modifies statements))
(define (empty-instruction-sequence)
 (make-instruction-sequence '() '() '()))

(define (compile-linkage linkage)
 (cond ((eq? linkage 'return)
        (make-instruction-sequence
         '(continue) '() (make-goto-statement '(reg continue))))
       ((eq? linkage 'next)
        (empty-instruction-sequence))
       (else
        (make-instruction-sequence
         '() '() (make-goto-statement (list 'label linkage))))))

(define (end-with-linkage linkage instruction-sequence)
 (preserving '(continue)
  instruction-sequence
  (compile-linkage linkage)))

(define (compile-self-evaluating exp target linkage)
 (end-with-linkage
  linkage
  (make-instruction-sequence
   '()
   (list target)
   (make-assignment-statement target (list 'const exp)))))

(define (compile-variable exp target linkage)
  (end-with-linkage
   linkage
   (make-instruction-sequence
    '(env)
    (list target)
    (make-assignment-statement target
                               (make-lookup-expression exp '(reg env))))))

(define (compile-definition exp target linkage)
  (let ((var (definition-variable exp))
        (get-value-code
         (compile (definition-value exp) '(reg val) 'next)))
    (end-with-linkage linkage
     (preserving '(env)
      get-value-code
      (make-instruction-sequence
       '(env val)
       (list target)
       (make-perform-statement
        "define_variable" var '(reg val) '(reg env)))))))

(define (last-exp? seq) (null? (cdr seq)))
(define (first-exp seq) (car seq))
(define (rest-exps seq) (cdr seq))

(define (compile-sequence seq target linkage)
  (if (last-exp? seq)
      (compile (first-exp seq) target linkage)
      (preserving '(env continue)
       (compile (first-exp seq) target 'next)
       (compile-sequence (rest-exps seq) target linkage))))

;;;; 5.5.4 Combining Instruction Sequences
(define (registers-needed s)
 (if (symbol? s) () (car s)))
(define (registers-modified s)
 (if (symbol? s) () (cadr s)))
(define (statements s)
 (if (symbol? s) (list s) (caddr s)))

(define (needs-register? seq reg)
 (memq reg (registers-needed seq)))
(define (modifies-register? seq reg)
 (memq reg (registers-modified seq)))

(define (append-2-sequences seq1 seq2)
  (make-instruction-sequence
   (list-union (registers-needed seq1)
               (list-difference (registers-needed seq2)
                                (registers-modified seq1)))
   (list-union (registers-modified seq1)
               (registers-modified seq2))
   (append (statements seq1) (statements seq2))))

(define (append-seq-list seqs)
  (if (null? seqs)
      (empty-instruction-sequence)
      (append-2-sequences (car seqs)
                          (append-seq-list (cdr seqs)))))

(define (append-instruction-sequences . seqs)
 (append-seq-list seqs))

(define (preserving regs seq1 seq2)
 (if (null? regs)
     (append-instruction-sequences seq1 seq2)
     (let ((first-reg (car regs)))
       (if (and (needs-register? seq2 first-reg)
                (modifies-register? seq1 first-reg))
           (preserving (cdr regs)
            (make-instruction-sequence
             (list-union (list first-reg)
                         (registers-needed seq1))
             (list-difference (registers-modified seq1)
                              (list first-reg))
             (append (make-save-statement first-reg)
                     (statements seq1)
                     (make-restore-statement first-reg)))
            seq2)
           (preserving (cdr regs) seq1 seq2)))))
