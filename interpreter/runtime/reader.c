/*
 * The reader
 */
#include <stdlib.h> /* for malloc */
#include <stdio.h> /* for fgets in main */
#include <string.h> /* for strdup and strpbrk */
#include <ctype.h> /* for isspace */

#include "reader.h"
#include "../storage.h"
#include "..lib/list.h"
#include "..lib/str_utils.h"

#define MAX_LINE 100

object_t parse_sexp2(char **in, object_t current);
enum kind next_token(char **, char **);
char *get_symbol(char **);
char *get_string(char **);

/* delimeters for symbols */
static char *punct = " ()'`,\"";

/* the different kinds of tokens in our subset of Scheme/Lisp, note
   that in this case "Symbol" is actually both Lisp symbols and numbers */

enum kind { Left, Right, Single, Back, Comma, Period, Symbol, String, End };

static char *input;

object_t read_file(char *filename) {
  FILE *f = fopen(filename, "r");
  char *input;
  object_t seq = cons(obj_new_symbol("begin"), NIL);
  while((input = read_sexp(f)) != NULL) {
    printf("input: [%s]%d\n", input, (int)strlen(input));
    object_t exp = parse_sexp(input);
    printf("exp: ");
    print_object(exp);
    printf("\n");
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
    rest = parse_sexp2(in, NULL); /* grabbing next symbol */
    set_cdr(storage_last(current), rest);
    return parse_sexp2(in, current);

  case Symbol:
  case String:
    if(k == Symbol)
      s = obj_new_symbol(buf);
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
}

/* assumes **ch is a " and reads up to the next (unescaped) ",
   returning a copy of the contents of the string */
char *get_string(char **ch) {
  (*ch)++;
  char *brk = strpbrk(*ch, "\"");
  while(*(brk-1) == '\\')
    brk = strpbrk(brk+1, "\"");
  int len = brk - *ch;
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

/* Read an s-expression from a FILE and buffer lines in a linked-list */
char *read_sexp(FILE *in) {
  struct list_t *head = NULL;
  int parens = 0;
  size_t chars = 0;

  char *buf = malloc(sizeof(*buf) * MAX_LINE);
  if(buf == NULL)
    return NULL;
  buf[0] = '\0';

  /* while there are valid lines and while the parens are not matched */
  char *str;
  while((str = fgets(buf, MAX_LINE, in)) != NULL) {
    
    if(buf[0] == '\n') /* skip totally blank lines */
      continue;

    strip_comments(buf);

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

  strip_comments(buf);
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
