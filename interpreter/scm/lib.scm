
(define (caar x) (car (car x)))
(define (cadr x) (car (cdr x)))
(define (cdar x) (cdr (car x)))
(define (cddr x) (cdr (cdr x)))

(define (not x)
  (if x #f #t))

(define (assoc x lst)
  (if (nil? lst)
      #f
      (if (eq? (caar lst) x)
          (car lst)
          (assoc x (cdr lst)))))

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

