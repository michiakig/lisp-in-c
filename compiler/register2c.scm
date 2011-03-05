
(load-with-macros "ch5-syntax.scm")
(load-with-macros "ch5-compiler.scm")
(load-with-macros "register-syntax.scm")

(define *labels* ())
(define *labels-count* 0)
(define (make-label-define name)
  (set! *labels-count* (+ 1 *labels-count*))
  (cons name *labels-count*))

(define (make-string x)
  (cond ((string? x) x)
        ((symbol? x) (symbol->string x))
        ((number? x) (number->string x))
        (else "?")))

(define (lisp->c exp)
  (set! *labels* ())
  (set! *labels-count* 0)
  
  (let ((compiled (compile exp 'val 'return)))
    ;; (for-each (lambda (label)
    ;;             (print "#define ")
    ;;             (print (car label))
    ;;             (print " ")
    ;;             (print (cdr label))
    ;;             (print "\n")))
    (map (lambda (inst)
           (apply string-append-n inst))
         (map (lambda (inst)
                (map make-string (flatten (inst->c inst))))
              (caddr compiled)))
    ;; (for-each (lambda (s)
    ;;             (print s))
    ;;           compiled)
    ))

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
        
        (else (list "???"))))

;; (test (op false?) (reg val)) -- > if(falsey(reg[val])) {flag=TRUE;}

(define (test->c inst)
  (append
   (list "if(")
   (opcall->c (opcall-op inst) (opcall-args inst))
   (list "){flag=TRUE;}\n")))

;; (branch (label foo)) --> if(flag){GOTO(label(foo));}
(define (branch->c inst)
  (list "if(flag){GOTO(" (label-exp-name (branch-target inst)) ");}\n"))

(define (save->c inst)
  (list "save(" (inst->c (save-arg inst)) ");\n"))

(define (restore->c inst)
  (list (inst->c (restore-target inst)) "=restore();\n"))

(define (perform->c inst)
  (append
   (opcall->c (perform-op inst) (perform-args inst))
   (list ";\n")))

(define (goto->c inst)
  (cond ((reg? (goto-target inst))
         (list "GOTO(obj2label(" (inst->c (goto-target inst)) "));\n"))
        ((label-exp? (goto-target inst))
         (list "GOTO(" (label-exp-name (goto-target inst)) ");\n"))
        (else (error "ERROR! bad goto target: " (goto-target inst)))))

(define (label->c inst)
  (if (label-stmt? inst)
      (begin
        (set! *labels* (cons (make-label-define inst) *labels*))
        (list "case " inst ":;\n"))
      (list "label2obj(" (label-exp-name inst) ")")))

(define (assign->c inst)
  (if (op-call? (car (assign-rest inst)))
      (append (list (reg->c (assign-target inst)) "=")
              (opcall->c (car (assign-rest inst)) (cdr (assign-rest inst)))
              (list ";\n"))
      (append (list (reg->c (assign-target inst)) "=")
              (inst->c (car (assign-rest inst)))
              (list ";\n"))))

(define (opcall->c op args)
  (append (list (op-name op) "(")
          (interleave "," (map inst->c args))
          (list ")")))

(define (reg->c name) (list "reg[" name "]"))

(define (const->c const)
  (let ((val (cadr const)))
    (cond ((number? val) (list "obj_new_number(" val ")"))
          ((symbol? val) (list "obj_new_symbol(\"" val "\")"))
          ((string? val) (list "obj_new_string(\"" val "\")"))
          ((cons? val)
           (list->cons-calls val)))))

; (a b c) --> "cons(a, cons(b, cons(c, NIL)))"
(define (list->cons-calls lst)
  (if (null? (cdr lst))
      (list "cons(" (car lst) ",NIL)")
      (append (list "cons(" (car lst) ",")
              (list->cons-calls (cdr lst))
              (list ")"))))
