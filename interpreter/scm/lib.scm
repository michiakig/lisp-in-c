
(define (caar x) (car (car x)))
(define (cadr x) (car (cdr x)))
(define (cdar x) (cdr (car x)))
(define (cddr x) (cdr (cdr x)))
(define (caaar x) (car (car (car x))))
(define (caadr x) (car (car (cdr x))))
(define (cadar x) (car (cdr (car x))))
(define (caddr x) (car (cdr (cdr x))))
(define (cdaar x) (cdr (car (car x))))
(define (cdadr x) (cdr (car (cdr x))))
(define (cddar x) (cdr (cdr (car x))))
(define (cdddr x) (cdr (cdr (cdr x))))


(define (list . rest) rest)

(define (length lst)
  (if (null? lst)
      0
      (+ 1 (length (cdr lst)))))

(define (not x)
  (if x #f #t))

(define (assoc x lst)
  (if (null? lst)
      #f
      (if (eq? (caar lst) x)
          (car lst)
          (assoc x (cdr lst)))))

(define (memq item lst)
  (if (null? lst)
      #f
      (if (eq? item (car lst))
          lst
          (memq item (cdr lst)))))

(define (append! l1 l2)
  (if (null? (cdr l1))
      (set-cdr! l1 l2)
      (append! (cdr l1) l2)))

(define (append l1 l2)
  (if (null? l1)
      l2
      (cons (car l1)
            (append (cdr l1) l2))))

(define (filter p lst)
  (if (null? lst)
      ()
      (if (p (car lst))
          (cons (car lst)
                (filter p (cdr lst)))
          (filter p (cdr lst)))))

(define (map fn lst)
  (if (null? lst)
      ()
      (cons (fn (car lst))
            (map fn (cdr lst)))))

(define (improper-map fn lst)
  (if (null? lst)
      ()
      (if (cons? lst)
          (cons (fn (car lst))
                (improper-map fn (cdr lst)))
          (fn lst))))

(define (reduce op lst acc)
  (if (null? lst)
      acc
      (reduce op (cdr lst) (op (car lst) acc))))

(define (for-each fn lst)
  (if (null? lst)
      'done
      (begin
        (fn (car lst))
        (for-each fn (cdr lst)))))

(define (reverse lst)
  (define (reverse-i acc lst)
    (if (null? lst)
        acc
        (reverse-i (cons (car lst) acc) (cdr lst))))
  (reverse-i () lst))

(define (position elt lst)
  (define (position-i n elt lst)
    (if (null? lst)
        #f
        (if (eq? elt (car lst))
            n
            (position-i (+ n 1) elt (cdr lst)))))
  (position-i 0 elt lst))
