#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read.h"
#include "aux.h"
#include "str_utils.h"

#include "storage.h"
#include "lib/stack.h"

#define MAX_LINE 1000

object_t get_symbol(char **);

/* Parses an s-expression (from read_sexp) into a linked-list which
   is laid out in the interpreter's managed memory */
object_t parse_sexp(struct node *lines) {
  char *ch;

  Stack_T unfinished = Stack_new();

  object_t head = NIL; /* head of the current list */
  object_t finished = NIL;

  int paren = 0;
  struct node *line = lines; /* current line */
  ch = (char*)line->data; /* current character */
  while(ch != NULL) { /* step over each char in input */

    if(*ch == '(') { /* start a new list */

      if(paren) /* save the current list on the stack */
        Stack_push(unfinished, head);

      head = NIL;
      paren = 1;
      ch++;
    } else if(*ch == ')') { /* finish the current list */
      paren = 0;
      ch++;

      if(!Stack_empty(unfinished)) { /* there are unfinished lists on
                                        the stack */
        paren = 1;
        finished = head;
        /* append the just finished list to the outer, unfinished list */
        head = Stack_pop(unfinished);

        if(nilp(head)) {
          head = cons(finished, NIL);

        } else
          storage_append(finished, head);
      }

    } else if(*ch == ' ' || *ch == '\t') { /* TODO check other
                                              whitespace? */
      ch++; /* skip whitespace */
    } else if(*ch == '\n') { /* end of this line, step to the next
                                node in the list of lines */
      line = line->next;
      if(line != NULL)
        ch = line->data;
      else
        ch = NULL;

    } else if(*ch == '\'') { /* quote macro */
      
    } else { /* get the symbol beginning with char at ch */
      object_t sym = get_symbol(&ch);
      if(sym != NULL) {
        if(paren)
          if(nilp(head))
            head = cons(sym, NIL);
          else
            storage_append(sym, head);
        else
          return sym;
      } else {
        printf("ERROR in parse_sexp: strpbrk returned NULL\n");
      }
    }
  }

  return head;
}

object_t get_symbol(char **ch) {
  char *brk, tmp;
  int len;
  object_t sym;
  brk = strpbrk(*ch, "() \n\t");
  if(brk != NULL) {
    len = brk - *ch;
    tmp = *brk;
    *brk = '\0';
    sym = obj_new_symbol(*ch);
    *brk = tmp;
    (*ch) += len;
    return sym;
  } else {
    return NULL;
  }
}

/* Read an s-expression from a FILE and buffer lines in a linked-list */
struct node *read_sexp(FILE *f) {
  char *buf;
  struct node *head = NULL;
  struct node *last = NULL;
  struct node *n;
  int count = 0;

  buf = malloc(sizeof(char) * MAX_LINE);
  if(buf == NULL)
    return NULL;
  char *fgts;

  /* while there are valid lines and while the parens are not matched */
  while((fgts = fgets(buf, MAX_LINE, f)) != NULL &&
        (count += count_parens(buf, MAX_LINE)) > 0) {

    n = malloc(sizeof(*n));
    n->next = NULL;
    n->data = buf;

    if(head == NULL) {
      head = n;
      last = head;
    } else {
      last = auxappend(n, last);
    }
    buf = malloc(sizeof(*buf) * MAX_LINE);
    if(buf == NULL) {
      auxfor_each(head, &auxfree_node);
      return NULL;
    }

  }

  if(fgts == NULL) {
    auxfor_each(head, &auxfree_node);
    free(buf);
    return NULL;
  }


  if(buf[0] != '\n') {
    n = malloc(sizeof(*n));
    n->next = NULL;
    n->data = buf;

    if(head == NULL) {
      head = n;
    } else {
      last = auxappend(n, last);
    }
  } else {
    head = malloc(sizeof(*head));
    head->data = NULL;
    head->next = NULL;
    free(buf);
  }

  return head;
}
