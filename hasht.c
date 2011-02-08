/*
  Code for a simple hash table adapted from K&R 2e Chapter 6
*/

#include <stdlib.h>
#include <string.h>
#include "hasht.h"

unsigned hash(char *s) {
  unsigned hashval;

  for (hashval = 0; *s != '\0'; s++)
    hashval = *s + 31 * hashval;

  return hashval % HASHSIZE;
}

struct nlist *lookup(char *s, struct nlist *table[]) {
  struct nlist *np;
  
  for (np = table[hash(s)]; np != NULL; np = np->next)
    if (strcmp(s, np->sym->name) == 0)
      return np;
  return NULL;
}

symbol *intern(char *name, struct nlist *table[]) {
  struct nlist *np;
  symbol *sym;
  unsigned hashval;
  
  if ((np = lookup(name, table)) == NULL) { /* hasn't been installed yet */
    np = (struct nlist *) malloc(sizeof(*np));
    if(np != NULL)
      sym = (symbol *) malloc(sizeof(*sym));
    else
      return NULL;
    if (sym == NULL || (sym->name = strdup(name)) == NULL)
      return NULL;

    np->sym = sym;
    hashval = hash(name);
    np->next = table[hashval];
    table[hashval] = np;
  } else
    sym = np->sym; /* return a pointer to the previously interned symbol */

  return sym;
}

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
