
A rudimentary Lisp written in C, inspired by the last two exercises in SICP. (http://mitpress.mit.edu/sicp/full-text/book/book-Z-H-35.html#%_thm_5.51)

Other inspiration from:
http://scheme2006.cs.uchicago.edu/11-ghuloum.pdf
http://www.iro.umontreal.ca/~boucherd/mslug/meetings/20041020/minutes-en.html
https://github.com/darius/ichbins

1. Doesn't handle two s-expression on a one line, ignores the second one.
3. Primitives don't check for arity, so (quote foo bar) is just foo, and not an error.
1. No tail-call optimization
1. No garbage collection
1. No user-defined macros, altho a macro system is in place for "system" macros
