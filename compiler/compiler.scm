
(load-with-macros "ch5-syntax.scm")
(load-with-macros "ch5-compiler.scm")
(load-with-macros "register-syntax.scm")

(load-with-macros "boilerplate.scm")
(load-with-macros "pretty-print.scm")

(define *labels* ())
(define *labels-count* 1) ; labels start with 2, since END=0, START=1

(define (run-visual-test exp)
  (print "\n-- ")
  (print exp)
  (print "\n\n")
  (pretty-print-reg (caddr (compile exp 'val 'return)))
  (print "\n")
  (pretty-print-c (lisp->c exp)))

(define (make-label-define name)
  (set! *labels-count* (+ 1 *labels-count*))
  (cons name *labels-count*))

(define (make-string x)
  (cond ((string? x) x)
        ((symbol? x) (symbol->string x))
        ((number? x) (number->string x))
        (else "?")))

(define (labels->defines labels)
  (map (lambda (label)
              (string-append-n
               "#define "
               (symbol->string (car label))
               " "
               (number->string (cdr label))
               "\newline"))
       labels))

(define (compile-and-emit exp filename)
  (pretty-print-c (lambda (s)
                    (file-append s filename))
                  (lisp->c (_expand_ exp))))

(define (lisp->c exp)
  (set! *labels* ())
  (set! *labels-count* 1)
  (let ((compiled (compile exp 'val 'return)))
    (surround-with-boilerplate 
     (map (lambda (inst)
            (apply string-append-n inst))
          (map (lambda (inst)
                 (map make-string (flatten (inst->c inst))))
               (caddr compiled)))
     *labels*)))

(define (insts->strings insts)
  (map (lambda (inst)
         (apply string-append-n (map make-string (flatten (inst->c inst)))))
       insts))

(define (inst->c inst)
  (cond ((assign? inst) (assign->c inst))
        ((label-stmt? inst) (label->c inst))
        ((goto? inst) (goto->c inst))
        ((perform? inst) (perform->c inst))
        ((save? inst) (save->c inst))
        ((restore? inst) (restore->c inst))
        ((test? inst) (test->c inst))
        ((branch? inst) (branch->c inst))
        
        ((label-exp? inst) (label->c inst))
        ((reg? inst) (reg->c (reg-name inst)))
        ((const? inst) (const->c inst))
        
        (else (error "ERROR unknown instruction!"))))

;; (test (op false?) (reg val)) -- > if(falsey(reg[val])) {flag=TRUE;}

(define (test->c inst)
  (append
   (list "flag=")
   (opcall->c (opcall-op inst) (opcall-args inst))
   (list ";\newline")))

;; (branch (label foo)) --> if(flag){GOTO(label(foo));}
(define (branch->c inst)
  (list "if(flag){GOTO(" (label-exp-name (branch-target inst)) ");}\newline"))

(define (save->c inst)
  (list "save(" (reg->c (save-arg inst)) ");\newline"))

(define (restore->c inst)
  (list (reg->c (restore-target inst)) "=restore();\newline"))

(define (perform->c inst)
  (append
   (opcall->c (perform-op inst) (perform-args inst))
   (list ";\newline")))

(define (goto->c inst)
  (cond ((reg? (goto-target inst))
         (list "GOTO(obj2label(" (inst->c (goto-target inst)) "));\newline"))
        ((label-exp? (goto-target inst))
         (list "GOTO(" (label-exp-name (goto-target inst)) ");\newline"))
        (else (error "ERROR! bad goto target: " (goto-target inst)))))

(define (label->c inst)
  (if (label-stmt? inst)
      (begin
        (set! *labels* (cons (make-label-define inst) *labels*))
        (list "case " inst ":;\newline"))
      (list "label2obj(" (label-exp-name inst) ")")))

(define (assign->c inst)
  (if (op-call? (car (assign-rest inst)))
      (append (list (reg->c (assign-target inst)) "=")
              (opcall->c (car (assign-rest inst)) (cdr (assign-rest inst)))
              (list ";\newline"))
      (append (list (reg->c (assign-target inst)) "=")
              (inst->c (car (assign-rest inst)))
              (list ";\newline"))))

(define (opcall->c op args)
  (append (list (op-name op) "(")
          (interleave "," (map inst->c args))
          (list ")")))

(define (reg->c name) (list "reg[" name "]"))

(define (const->c const)
  (let ((val (cadr const)))
    (cond ((number? val) (list "obj_new_number(" val ")"))
          ((symbol? val) (list "obj_new_symbol(\quote" val "\quote)"))
          ((string? val) (list "obj_new_string(\quote" val "\quote)"))
          ((null? val) (list "NIL"))
          ((cons? val)
           (list->cons-calls val)))))

; (a b c) --> "cons(a, cons(b, cons(c, NIL)))"
(define (list->cons-calls lst)
  (if (null? (cdr lst))
      (list "cons(obj_new_symbol(\quote" (car lst) "\quote),NIL)")
      (append (list "cons(obj_new_symbol(\quote" (car lst) "\quote),")
              (list->cons-calls (cdr lst))
              (list ")"))))
