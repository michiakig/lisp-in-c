#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "llist.h"
#include "str_utils.h"
#include "read.h"
#include "eval.h"

#define MAX_LEN 100

/* parse an s-expression into a linked-list (actually a tree) */
DATA *parse_sexp(LLIST *lines) { // lines is the output of read_sexp
  char *buf;
  char *brk;
  int len;

  LLIST *stack = NULL; // stack of unfinished lists

  LLIST *head = NULL; // head of the current list
  LLIST *last = NULL; // last of the current list

  LLIST *line = lines; // current line
  LLIST *old_head;
  LLIST *old_last;

  DATA *ret = NULL;

  char *ch = (char*)line->data; // current char
  while(ch != NULL) {

    if(*ch=='(') { // start up a new list
      ch++;
      if(ret == NULL) {
        ret = malloc(sizeof(DATA));
        ret->type = LIST;
      }
      if(last != NULL) {
        push(&stack, last, 0);
        push(&stack, head, 0);
      }
      last = NULL;
      head = last;
    } else if(*ch==')') { // finish the current list 
      ch++;
      if(stack != NULL) {
        old_head = pop(&stack);
        old_last = pop(&stack);
        last = list_append(old_last, head, 0); // append to the outer list
        head = old_head;
      }
      if(head == NULL) {
        head = malloc(sizeof(LLIST));
        head->primitive = 1;
        head->data = NULL;
        last = head;
        ret->data = head;
      }
    } else if(*ch==' ' || *ch=='\t') {
      ch++; // skip over whitespace
    } else if(*ch=='\n') {
      line = line->next;
      if(line != NULL)
        ch=line->data;
      else
        ch=NULL;
    } else {
      brk = strpbrk(ch, "() \n\t");
      if(brk != NULL) {

        len = brk - ch;
        buf = (char*)malloc(sizeof(char)*len+1);
        int i=0;
        for(i=0; i<len; i++)
          buf[i]=*(ch+i);
        buf[i]='\0';

        if(last == NULL) { // this is the first element of the list
          if(ret == NULL) {
            ret = malloc(sizeof(DATA));
            ret->type = ATOM;
            ret->data = buf;
            return ret;
          } else { // not the first element in the list
            last = (LLIST*)malloc(sizeof(LLIST));
            last->data = buf;
            last->primitive = 1;
            last->next = NULL;
            head = last;
            if(ret->data == NULL) { // ret hasn't been initialized by an outer list
              ret->data = head;
              ret->type = LIST;
            }
          }
        } else { // adding an new item to the list
          last = list_append(last, buf, 1);
        }
        ch = brk;
      } else {
        printf("ERROR in parse_sexp: strpbrk returned NULL\n");
      }
    }
  }
  
  return ret;
}

/* read an s-expression from a file, buffer lines in a linked-list */
LLIST *read_sexp(FILE *f) {
  char *buf = (char*)malloc(sizeof(char)*(MAX_LEN+1));
  LLIST *head = NULL;
  LLIST *last = NULL;
  int count=0;

  while(fgets(buf, MAX_LEN+1, f) != NULL &&
        (count+=count_parens(buf, MAX_LEN+1)) > 0) {
    if(head == NULL) {
      head = (LLIST*)malloc(sizeof(LLIST));
      last = head;
      head->data = buf;
      head->primitive = 1;
      head->next = NULL;
    } else {
      last = list_append(last, buf, 1);
    }
    buf = (char*)malloc(sizeof(char)*(MAX_LEN+1));
  }

  if(head == NULL) {
    if(strlen(buf) != 0) {
      head = (LLIST*)malloc(sizeof(LLIST));
      head->primitive = 1;
      head->data = buf;
      head->next = NULL;
    }
  } else {
    last = list_append(last, buf, 1);
  }

  return head;
}
