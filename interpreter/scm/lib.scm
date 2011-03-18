
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

(define (caaaar x) (car (car (car (car x)))))
(define (caaadr x) (car (car (car (cdr x)))))
(define (caadar x) (car (car (cdr (car x)))))
(define (caaddr x) (car (car (cdr (cdr x)))))
(define (cadaar x) (car (cdr (car (car x)))))
(define (cadadr x) (car (cdr (car (cdr x)))))
(define (caddar x) (car (cdr (cdr (car x)))))
(define (cadddr x) (car (cdr (cdr (cdr x)))))
(define (cdaaar x) (cdr (car (car (car x)))))
(define (cdaadr x) (cdr (car (car (cdr x)))))
(define (cdadar x) (cdr (car (cdr (car x)))))
(define (cdaddr x) (cdr (car (cdr (cdr x)))))
(define (cddaar x) (cdr (cdr (car (car x)))))
(define (cddadr x) (cdr (cdr (car (cdr x)))))
(define (cdddar x) (cdr (cdr (cdr (car x)))))
(define (cddddr x) (cdr (cdr (cdr (cdr x)))))

(define pair? cons?)
(define false '#f)
(define true '#t)

(define (boolean? x)
  (if (eq? x false)
      #t
      (if (eq? x true)
          #t
          #f)))

(define (display x) (print x))
(define (newline) (print "\newline"))

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

(define (append l . rest)
  (if (null? rest)
      l
      (if (null? (cdr rest))
          (append2 l (car rest))
          (apply append (cons (append2 l (car rest))
                              (cdr rest))))))

(define (append2 l1 l2)
  (if (null? l1)
      l2
      (cons (car l1)
            (append2 (cdr l1) l2))))

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

(define (string-append-n s . args)
  (if (null? args)
      s
      (if (null? (cdr args))
          (string-append s (car args))
          (apply string-append-n (cons (string-append s (car args))
                                       (cdr args))))))
(define (flatten lst)
  (if (null? lst)
      ()
      (if (cons? (car lst))
          (append (flatten (car lst))
                  (flatten (cdr lst)))
          (cons (car lst)
                (flatten (cdr lst))))))
