#include <ctype.h> /* for isdigit */

#include "str_utils.h"

int count_parens(char *s, int len) {
  int count=0;
  for(int i=0; i<len && s[i]!='\0'; i++) {
    if(s[i]=='(') {
      count++;
    } else if(s[i]==')') {
      count--;
    }
  }
  return count;
}

int contains(char c, char *s) {
  int i=0;
  while(s[i]!='\0' && s[i]!=c)
    i++;
  if(s[i]=='\0')
    return 0;
  else
    return 1;
}

void remove_newline(char *s) {
  int i=0;
  while(s[i]!='\n' && s[i]!='\0')
    i++;

  s[i] = '\0';
}

int replace_first(char *s, char old, char new) {
  int i=0;
  int count=0;
  while(s[i]!='\0') {
    if(s[i]==old) {
      s[i]=new;
      count++;
      break;
    }
    i++;
  }
  return count;
}

int replace_all(char *s, char old, char new) {
  int i=0;
  int count=0;
  while(s[i]!='\0') {
    if(s[i]==old) {
      s[i]=new;
      count++;
    }
    i++;
  }
  return count;
}

int all_digits(char *s) {

  for(char* ch = s; *ch != '\0'; ch++) {
    if(isdigit(*ch) != 1) {
      return 0;
    }
  }

  return 1;
}
