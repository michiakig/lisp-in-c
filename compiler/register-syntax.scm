(define (assign? inst) (tagged-list? inst 'assign))
(define (assign-target inst) (cadr inst))
(define (assign-rest inst) (cddr inst))

(define (op-call? inst) (tagged-list? inst 'op))
(define (op-name inst) (cadr inst))
(define (opcall-op inst) (cadr inst))
(define (opcall-args inst) (cddr inst))

(define (goto? inst) (tagged-list? inst 'goto))
(define (goto-target inst) (cadr inst))

(define (test? inst) (tagged-list? inst 'test))

(define (branch? inst) (tagged-list? inst 'branch))
(define (branch-target inst) (cadr inst))

(define (save? inst) (tagged-list? inst 'save))
(define (save-arg inst) (cadr inst))

(define (restore? inst) (tagged-list? inst 'restore))
(define (restore-target inst) (cadr inst))

(define (perform? inst) (tagged-list? inst 'perform))
(define (perform-op inst) (cadr inst))
(define (perform-args inst) (cddr inst))

(define (label-stmt? inst) (symbol? inst))
(define (label-exp? inst) (tagged-list? inst 'label))
(define (label-exp-name inst) (cadr inst))

(define (const? inst) (tagged-list? inst 'const))

(define (reg? inst) (tagged-list? inst 'reg))
(define (reg-name inst) (cadr inst))
