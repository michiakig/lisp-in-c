#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "list.h"
#include "str_utils.h"

#include "read.h"

#define MAX_LINE 1000

/* Parses an s-expression (from read_sexp) into a linked-list (actually a tree) */
list *parse_sexp(list *lines) {
  char *buf;
  char *brk;
  int len;

  list *stack = NULL; // stack of unfinished lists

  list *head = NULL; // head of the current list
  list *last = NULL; // last of the current list

  list *hcontainer; // containers for pushing unfinished lists on the stack
  list *lcontainer;


  list *old_head;
  list *old_last;

  list *ret = NULL;

  list *line = lines; // current line
  if(line->type != Atom)
    return NULL;
  char *ch = (char*)line->kindData.atomData; // current char

  while(ch != NULL) {

    if(*ch == '(') { // start a new list
      ch++;

      if(ret == NULL) { // haven't initialized the final return list
        ret = malloc(sizeof(list));
        init_list(ret, NULL, List, NULL);
      }

      if(last != NULL) { // save the current, unfinished list on the stack
        lcontainer = malloc(sizeof(list));
        init_list(lcontainer, NULL, List, last);
        hcontainer = malloc(sizeof(list));
        init_list(hcontainer, NULL, List, head);
        push(lcontainer, &stack);
        push(hcontainer, &stack);
      }

      last = NULL;
      head = last;
    } else if(*ch == ')') { // finish the current list 
      ch++;

      if(stack != NULL) { // there are unfinished lists on the stack
        hcontainer = pop(&stack);
        lcontainer = pop(&stack);
        list *n = malloc(sizeof(list));
        init_list(n, NULL, List, head);
        // append the just finished list to the outer, unfinished list
        last = append(n, lcontainer->kindData.listData);
        head = hcontainer->kindData.listData;
      }

      if(head == NULL) { // empty list
        head = malloc(sizeof(list));
        init_list(head, NULL, Atom, NULL);
        last = head;
        init_list(ret, NULL, List, head);
      }
    } else if(*ch == ' ' || *ch == '\t') { // TODO check other whitespace?
      ch++; // skip whitespace
    } else if(*ch == '\n') { // end of this line, step to the next
                             // node in the list of lines
      line = line->next;
      if(line != NULL)
        ch = line->kindData.atomData;
      else
        ch = NULL;
    } else { // get the token beginning with char at ch
      brk = strpbrk(ch, "() \n\t");
      if(brk != NULL) {
        len = brk - ch;
        buf = (char*)malloc(sizeof(char)*len+1);
        
        int i = 0;
        for(i = 0; i < len; i++)
          buf[i] = *(ch+i);
        buf[i] = '\0';

        if(last == NULL) { // this is the first element of the list
          if(ret == NULL) { // and the only item, ie haven't seen an open paren
            ret = malloc(sizeof(list));
            init_list(ret, NULL, Atom, buf);
            return ret;
          } else { // not the first element in the list
            last = (list*)malloc(sizeof(list));
            init_list(last, NULL, Atom, buf);
            head = last;
            if(ret->kindData.listData == NULL) { // ret hasn't been used yet
              init_list(ret, NULL, List, head);
            }
          }
        } else { // adding an new item to the list
          list *n = malloc(sizeof(list));
          init_list(n, NULL, Atom, buf);
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
list *read_sexp(FILE *f) {
  char *buf = malloc(sizeof(char) * MAX_LINE);
  if(buf == NULL)
    return NULL;

  list *head = NULL;
  list *last = NULL;
  list *n;
  int count = 0;
  /* while there are valid lines and while the parens are not matched */
  while(fgets(buf, MAX_LINE, f) != NULL &&
        (count += count_parens(buf, MAX_LINE)) > 0) {

    n = malloc(sizeof(list));
    init_list(n, NULL, Atom, buf);

    if(head == NULL) {
      head = n;
      last = head;
    } else {
      last = append(n, last);
    }
    buf = malloc(sizeof(char) * MAX_LINE);
    if(buf == NULL) {
      simple_rfree(head);
      return NULL;
    }
  }

  if(strlen(buf) != 0) {
    n = malloc(sizeof(list));
    init_list(n, NULL, Atom, buf);
    if(head == NULL) {
      head = n;
    } else {
      last = append(n, last);
    }
  }

  return head;
}
