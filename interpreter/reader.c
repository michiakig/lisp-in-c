/*
 * The reader
 */
#include <stdlib.h> /* for malloc */
#include <stdio.h> /* for fgets in main */
#include <string.h> /* for strdup and strpbrk */
#include <ctype.h> /* for isspace */

#include "reader.h"
#include "storage.h"

#include "lib/list.h"
#include "lib/str_utils.h"

#define MAX_LINE 200

object_t wrap(char **in, object_t current, char *type);
object_t parse_sexp2(char **in, object_t current);
enum kind next_token(char **, char **);
char *get_symbol(char **);
char *get_string(char **);

/* the different kinds of tokens in our subset of Scheme/Lisp, note
   that in this case "Symbol" is actually both Lisp symbols and numbers */

enum kind { Left, Right, Single, Back, Comma, Period, Symbol, String, End };

static char *input;

object_t read_file(char *filename) {
  FILE *f = fopen(filename, "r");
  char *input;
  object_t seq = cons(obj_new_symbol("begin"), NIL);
  while((input = read_sexp(f)) != NULL) {
    object_t exp = parse_sexp(input);
    storage_append(exp, seq);
  }
  return seq;
}

object_t read_stream(FILE *stream) {
  input = read_sexp(stream);
  return parse_sexp(input);  
}

object_t parse_sexp(char *in) {
  return parse_sexp2(&in, NULL);
}

object_t parse_sexp2(char **in, object_t current) {
  char *buf;
  enum kind k = next_token(in, &buf);
  object_t rest, s;
  switch(k) {

  case End:
    return current;

  case Left:
    rest = parse_sexp2(in, NIL);
    if(current == NULL)
      return rest;
    else if(current == NIL)
      current = cons(rest, NIL);
    else
      storage_append(rest, current);

    return parse_sexp2(in, current);
    
  case Right:
    return current;

  case Period:
    rest = parse_sexp2(in, NULL);
    set_cdr(storage_last(current), rest);
    return parse_sexp2(in, current);

  
  case Single: /* following are reader-macros */
    return wrap(in, current, "quote");
  case Back:
    return wrap(in, current, "quasiquote");
  case Comma:
    return wrap(in, current, "unquote");

  case Symbol:
  case String:
    if(k == Symbol) {
      if(all_digits(buf))
        s = obj_new_number(atoi(buf));
      else
        s = obj_new_symbol(buf);
    }
    else if(k == String)
      s = obj_new_string(buf);

    if(current == NULL)
      return s;
    else if(current == NIL)
      return parse_sexp2(in, cons(s, NIL));
    else {
      storage_append(s, current);
      return parse_sexp2(in, current);
    }
  }
  return NULL;
}

object_t wrap(char **in, object_t current, char *type) {
    object_t rest = parse_sexp2(in, NULL);
    object_t ret = cons(obj_new_symbol(type), cons(rest, NIL));

    if(current == NULL) {
      return ret;
    } else if(current == NIL) {
      current = cons(ret, NIL);
    } else {
      storage_append(ret, current);
    }

    return parse_sexp2(in, current);
}



/* assumes **ch is a " and reads up to the next (unescaped) ",
   returning a copy of the contents of the string */
char *get_string(char **ch) {
  (*ch)++;
  char *brk = strpbrk(*ch, "\"");
  while(*(brk-1) == '\\')
    brk = strpbrk(brk+1, "\"");

  *brk = '\0';
  char *string = strdup(*ch);
  *brk = '"';
  (*ch) = brk+1;
  return string;
}

/* assumes that **ch is the beginning of a symbol, and returns a copy
   of the symbol */
char *get_symbol(char **ch) {
  char *brk, tmp;
  int len;
  brk = strpbrk(*ch, "() \n\t");
  len = brk - *ch;
  tmp = *brk;
  *brk = '\0';
  char *symbol = strdup(*ch);
  *brk = tmp;
  (*ch) += len;
  return symbol;
}

/* reads the next token pointed to by *in, returns the type and
   stores Symbols and Strings in *buf */
enum kind next_token(char **in, char **buf) {
  switch(**in) {

  case '\0':
    return End;

  case '(':
    (*in)++;
    return Left;
    break;

  case ')':
    (*in)++;
    return Right;
    break;

  case '\'':
    (*in)++;
    return Single;
    break;

  case '`':
    (*in)++;
    return Back;
    break;

  case ',':
    (*in)++;
    return Comma;
    break;

  case '.':
    (*in)++;
    return Period;
    break;

  default:
    if(isspace(**in)) {
      (*in)++;
      return next_token(in, buf);
    } else if(**in == '"') {
      *buf = get_string(in);
      return String;
    } else {
      *buf = get_symbol(in);
      return Symbol;
    }
    break;
  }
}

/* strips comments from a line, accepts an int indicating if we are
   inside a string, returns if we are still inside a string */
int strip_comments(char *s, int quote) {
  while(*s != '\0') {
    if(*s == '"')
      quote = quote ? 0 : 1;
    if(*s == ';' && !quote)
      break;
    s++;
  }
  *s = '\0';
  return quote;
}

/* Read an s-expression from a FILE and buffer lines in a linked-list */
char *read_sexp(FILE *in) {
  struct list_t *head = NULL;
  int parens = 0;
  int quote = 0;

  char *buf = malloc(sizeof(*buf) * MAX_LINE);
  if(buf == NULL)
    return NULL;
  buf[0] = '\0';

  /* while there are valid lines and while the parens are not matched */
  char *str;
  while((str = fgets(buf, MAX_LINE, in)) != NULL) {

    quote = strip_comments(buf, quote);
    if(buf[0] == '\n' || buf[0] == '\0') /* skip totally blank lines */
      continue;

    /* break if we've read a matched s-expression */
    if((parens += count_parens(buf, MAX_LINE)) == 0)
      break;

    head = list_push(buf, head);

    buf = malloc(sizeof(*buf) * MAX_LINE);
    if(buf == NULL) {
      list_for_each(head, &list_free_node);
      return NULL;
    }
  }

  if(str == NULL) {
    list_for_each(head, &list_free_node);
    free(buf);
    return NULL;
  }

  quote = strip_comments(buf, quote);
  head = list_push(buf, head);
  head = list_reverse(head);

  size_t len = 0;
  struct list_t *n;
  for(n = head; n != NULL; n = n->next)
    len += strlen((char *) n->data);

  char *concat = malloc(sizeof(*concat) * (len+1));
  char *i = concat;
  for(n = head; n != NULL; n = n->next) {
    len = strlen(n->data);
    strncpy(i, (char*)n->data, len);
    i += len;
  }
  *i = '\0';
  list_for_each(head, &list_free_node);
  return concat;
}
