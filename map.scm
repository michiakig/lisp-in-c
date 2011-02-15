
(define map
  (lambda (fn lst)
    (if (nil? (cdr lst))
        (cons (fn (car lst)) nil)
        (cons (fn (car lst))
              (map fn (cdr lst))))))
