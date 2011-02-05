#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "str_utils.h"

#include "read.h"

#define MAX_LEN 100

/* Parses an s-expression into a linked-list (actually a tree) */
LIST *parse_sexp(LIST *lines) { // lines is the output of read_sexp
  char *buf;
  char *brk;
  int len;

  LIST *stack = NULL; // stack of unfinished lists

  LIST *head = NULL; // head of the current list
  LIST *hcontainer;
  LIST *last = NULL; // last of the current list
  LIST *lcontainer;

  LIST *line = lines; // current line
  LIST *old_head;
  LIST *old_last;

  LIST *ret = NULL;

  char *ch = (char*)line->data; // current char
  while(ch != NULL) {

    if(*ch=='(') { // start up a new list
      ch++;
      if(ret == NULL) {
        ret = malloc(sizeof(LIST));
        ret->type = CONS;
      }
      if(last != NULL) {
        lcontainer = malloc(sizeof(LIST));
        lcontainer->data = last;
        lcontainer->next = NULL;
        lcontainer->type = CONS;

        hcontainer = malloc(sizeof(LIST));
        hcontainer->data = head;
        hcontainer->next = NULL;
        hcontainer->type = CONS;

        push(&stack, lcontainer);
        push(&stack, hcontainer);
      }
      last = NULL;
      head = last;
    } else if(*ch==')') { // finish the current list 
      ch++;
      if(stack != NULL) {
        hcontainer = pop(&stack);
        lcontainer = pop(&stack);
        LIST *n = malloc(sizeof(LIST));
        n->type = CONS;
        n->data = head;
        n->next = NULL;
        last = append(lcontainer->data, n); // append to the outer list
        head = hcontainer->data;
      }
      if(head == NULL) {
        head = malloc(sizeof(LIST));
        head->type = ATOM;
        head->data = NULL;
        last = head;
        ret->data = head;
        ret->type = CONS;
        ret->next = NULL;
      }
    } else if(*ch==' ' || *ch=='\t') {
      ch++; // skip over whitespace
    } else if(*ch=='\n') {
      line = line->next;
      if(line != NULL)
        ch=((LIST*)line->data)->data;
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
          if(ret == NULL) { // only item, haven't seen an open paren
            ret = malloc(sizeof(LIST));
            ret->type = ATOM;
            ret->data = buf;
            ret->next = NULL;
            return ret;
          } else { // not the first element in the list
            last = (LIST*)malloc(sizeof(LIST));
            last->data = buf;
            last->type = ATOM;
            last->next = NULL;
            head = last;
            if(ret->data == NULL) { // ret hasn't been initialized by an outer list
              ret->data = head;
              ret->type = CONS;
            }
          }
        } else { // adding an new item to the list
          LIST *n = malloc(sizeof(LIST));
          n->next = NULL;
          n->type = ATOM;
          n->data = buf;
          last = append(last, n);
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
LIST *read_sexp(FILE *f) {
  char *buf = (char*)malloc(sizeof(char)*(MAX_LEN+1));
  LIST *head = NULL;
  LIST *last = NULL;
  LIST *n;
  int count=0;

  while(fgets(buf, MAX_LEN+1, f) != NULL &&
        (count+=count_parens(buf, MAX_LEN+1)) > 0) {

    n = malloc(sizeof(LIST));
    n->data = buf;
    n->type = ATOM;
    n->next = NULL;
    if(head == NULL) {
      head = n;
      last = head;
    } else {
      last = append(last, n);
    }
    buf = malloc(sizeof(char)*(MAX_LEN+1));
  }

  if(strlen(buf) != 0) {
    n = malloc(sizeof(LIST));
    n->data = buf;
    n->type = ATOM;
    n->next = NULL;
    if(head == NULL) {
      head = n;
    } else {
      last = append(last, n);
    }
  }

  return head;
}
