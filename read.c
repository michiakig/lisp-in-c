#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read.h"
#include "aux.h"
#include "str_utils.h"

#include "list.h"
#include "hasht.h"

#define MAX_LINE 1000

/* Parses an s-expression (from read_sexp) into a linked-list (actually a tree) */
list *parse_sexp(struct node *lines, struct nlist *obarray[]) {
  char *ch;
  
  /* used in extracting symbols */
  char *brk, tmp;
  int len;
  symbol *sym;

  struct node *line = lines; /* current line */
  struct node *stack = NULL; /* stack of unfinished lists */
  struct node *hcontainer; 
  struct node *lcontainer; /* containers for pushing unfinished lists
                              on the stack */

  list *head = NULL; /* head of the current list */
  list *last = NULL; /* last of ... */
  list *ret = NULL; /* container to return */
  list *n;

  ch = (char*)line->data;
  while(ch != NULL) { /* step over each char in input */

    if(*ch == '(') { /* start a new list */
      ch++;

      if(ret == NULL) { /* haven't initialized the final return list */
        ret = malloc(sizeof(list));
        init_list(ret, NULL, List, NULL);
      }

      if(last != NULL) { /* save the current, unfinished list on the 
                            stack */

        lcontainer = malloc(sizeof(*lcontainer));
        lcontainer->next = NULL;
        lcontainer->data = last;

        hcontainer = malloc(sizeof(*hcontainer));
        hcontainer->next = NULL;
        lcontainer->data = head;

        auxpush(lcontainer, &stack);
        auxpush(hcontainer, &stack);
      }

      last = NULL;
      head = last;
    } else if(*ch == ')') { /* finish the current list */
      ch++;

      if(stack != NULL) { /* there are unfinished lists on the stack */
        hcontainer = auxpop(&stack);
        lcontainer = auxpop(&stack);

        n = malloc(sizeof(list));
        init_list(n, NULL, List, head);
        /* append the just finished list to the outer, unfinished list */
        last = append(n, (list*)lcontainer->data);
        head = hcontainer->data;

        free(hcontainer);
        free(lcontainer);
      }

      if(head == NULL) { /* empty list */
        head = malloc(sizeof(*head));
        init_list(head, NULL, Symbol, NULL);
        last = head;
        init_list(ret, NULL, List, head);
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
        sym = intern(ch, obarray);
        *brk = tmp;

        if(last == NULL) { /* this is the first element of the list */
          if(ret == NULL) { /* and the only item, ie haven't seen an
                               open paren */
            ret = malloc(sizeof(*ret));
            init_list(ret, NULL, Symbol, sym);
            return ret;
          } else { /* not the first element in the list */
            last = (list*)malloc(sizeof(*last));
            init_list(last, NULL, Symbol, sym);
            head = last;
            if(ret->data.listData == NULL) { /* ret hasn't been used
                                                yet */
              init_list(ret, NULL, List, head);
            }
          }
        } else { /* adding an new item to the list */
          n = malloc(sizeof(*n));
          init_list(n, NULL, Symbol, sym);
          last = append(n, last);
        }
        ch = brk;
      } else {
        printf("ERROR in parse_sexp: strpbrk returned NULL\n");
      }
    }
  }
  return ret;
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
