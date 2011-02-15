
(define filter
  (lambda (p lst)
    (if (nil? lst)
        nil
        (if (p (car lst))
            (cons (car lst)
                  (filter p (cdr lst)))
            (filter p (cdr lst))))))
