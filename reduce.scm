
(define reduce
  (lambda (op lst acc)
    (if (nil? lst)
        acc
        (reduce op (cdr lst) (op (car lst) acc)))))

