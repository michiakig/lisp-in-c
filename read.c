#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read.h"
#include "aux.h"
#include "str_utils.h"

#include "storage.h"
#include "lib/stack.h"

#define MAX_LINE 1000

/* Parses an s-expression (from read_sexp) into a linked-list which
   is laid out in the interpreter's managed memory */
object_t parse_sexp(struct node *lines) {
  char *ch;
  /* used in extracting symbols */
  char *brk, tmp;
  int len;
  object_t sym;

  Stack_T unfinished = Stack_new();

  object_t head = NULL; /* head of the current list */
  object_t last = NULL; /* last of ... */
  object_t finished = NULL;
  int paren = 0;
  struct node *line = lines; /* current line */
  ch = (char*)line->data; /* current character */
  while(ch != NULL) { /* step over each char in input */

    if(*ch == '(') { /* start a new list */
      paren = 1;
      ch++;
      if(last != NULL) { /* save the current list on the stack */
        Stack_push(unfinished, last);
        Stack_push(unfinished, head);
      }
      last = NULL;
      head = last;
    } else if(*ch == ')') { /* finish the current list */
      paren = 0;
      ch++;

      if(head == NULL) { /* this was an empty list */
        head = NIL;
        last = head;
      }

      if(!Stack_empty(unfinished)) { /* there are unfinished lists on
                                        the stack */
        paren = 1;
        finished = head;
        /* append the just finished list to the outer, unfinished list */
        head = Stack_pop(unfinished);
        last = Stack_pop(unfinished);
        last = storage_append(finished, last);
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

    } else { /* get the token beginning with char at ch */
      brk = strpbrk(ch, "() \n\t");
      if(brk != NULL) {
        len = brk - ch;
        tmp = *brk;
        *brk = '\0';
        sym = obj_new_symbol(ch);
        *brk = tmp;

        if(last == NULL) { /* this is the first element of the list */
          if(paren == 0) { /* and it's the only item, ie just a atom */
            return sym;
          } else {
            last = cons(sym, NIL);
            head = last;
          }
        } else { /* adding an new item to the list */
          last = storage_append(sym, last);
        }
        ch = brk;
      } else {
        printf("ERROR in parse_sexp: strpbrk returned NULL\n");
      }
    }
  }
  return head;
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

  /* while there are valid lines and while the parens are not matched */
  while(fgets(buf, MAX_LINE, f) != NULL &&
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
    free(buf);
  }

  return head;
}
