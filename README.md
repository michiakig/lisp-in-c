
A rudimentary Lisp written in C, inspired by the last [two exercises](http://mitpress.mit.edu/sicp/full-text/book/book-Z-H-35.html#%_thm_5.51) in SICP. 

At the moment, the interpreter can run the compiler from SICP chapter 5 but:

- The repl doesn't handle two s-expression on a one line, ignores the second one.
- Primitives don't check for arity, so (quote foo bar) is just foo, and not an error.
- No tail-call optimization
- No garbage collection
- No user-defined macros, altho a macro system is in place for "system" macros

Indirect inspiration from:

[An Incremental Approach to Compiler Construction](http://scheme2006.cs.uchicago.edu/11-ghuloum.pdf)

[90 Minute Scheme to C Compiler](http://www.iro.umontreal.ca/~boucherd/mslug/meetings/20041020/minutes-en.html)

[A tiny self-hosting Lisp-to-C compiler](https://github.com/darius/ichbins)


