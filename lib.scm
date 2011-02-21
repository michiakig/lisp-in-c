
(define (cadr x) (car (cdr x)))
(define (cdar x) (cdr (car x)))
(define (cddr x) (cdr (cdr x)))

(define (caddr x) (car (cdr (cdr x))))
(define (cdadr x) (cdr (car (cdr x))))
(define (cddar x) (cdr (cdr (car x))))

(define (list . rest)
  rest)

(define (append! l1 l2)
  (if (nil? (cdr l1))
      (set-cdr! l1 l2)
      (append! (cdr l1) l2)))

(define (append l1 l2)
  (if (nil? (cdr l1))
      (cons (car l1) l2)
      (cons (car l1)
            (append (cdr l1) l2))))



(define (filter p lst)
  (if (nil? lst)
      ()
      (if (p (car lst))
          (cons (car lst)
                (filter p (cdr lst)))
          (filter p (cdr lst)))))

(define (map fn lst)
  (if (nil? (cdr lst))
      (cons (fn (car lst)) ())
      (cons (fn (car lst))
            (map fn (cdr lst)))))

(define (reduce op lst acc)
  (if (nil? lst)
      acc
      (reduce op (cdr lst) (op (car lst) acc))))

