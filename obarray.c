/* Code for a simple hash table adapted from K&R 2e Chapter 6 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obarray.h"

#define HASHSIZE 101

void print_obarray();
unsigned hash(char *s);
symbol_t lookup(char *s);

static struct symbol_t *obarray[HASHSIZE] = {NULL};

struct symbol_t {
  struct symbol_t *next;
  char *name;
};

unsigned hash(char *s) {
  unsigned hashval;

  for (hashval = 0; *s != '\0'; s++)
    hashval = *s + 31 * hashval;

  return hashval % HASHSIZE;
}

symbol_t lookup(char *s) {
  struct symbol_t *symp;
  for (symp = obarray[hash(s)]; symp != NULL; symp = symp->next)
    if (strcmp(s, symp->name) == 0)
      return symp;
  return NULL;
}

char *name(symbol_t sym) {
  return sym->name;
}

symbol_t intern(char *name) {
  struct symbol_t *symp;
  unsigned hashval;
  
  if ((symp = lookup(name)) == NULL) { /* hasn't been installed yet */
    symp = malloc(sizeof(*symp));
    if (symp == NULL)
      return NULL;
    if ((symp->name = strdup(name)) == NULL)
      return NULL;

    hashval = hash(symp->name);
    symp->next = obarray[hashval];
    obarray[hashval] = symp;
  }

  return symp;
}

/*
void free_hasht(struct nlist *table[]) {
  int i;
  struct nlist *np;
  struct nlist *tmp;

  for (i = 0; i<HASHSIZE; i++) {
    np = table[i];
    while (np != NULL) {
      free(np->sym->name);
      free(np->sym);
      tmp = np->next;
      free(np);
      np = tmp;
    }
  }
}
*/

void print_obarray() {
  int i;
  for(i = 0; i < HASHSIZE; i++) {
    if(obarray[i] != NULL) {
      struct symbol_t *s;
      for(s = obarray[i]; s; s = s->next) {
        printf("{%s}", s->name);
        if(s->next != NULL)
          printf("->");
      }
      printf("\n");
    }
  }
}
