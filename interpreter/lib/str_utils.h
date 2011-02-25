#ifndef __str_utils__
#define __str_utils__ 1

void strip_comments(char *s);
int count_parens(char *s, int len);
int contains(char c, char *s);

int replace_all(char *s, char old, char new);
int replace_first(char *s, char old, char new);

int all_digits(char *s);

void remove_newline(char *s);
#endif
