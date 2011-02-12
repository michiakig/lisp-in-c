#ifndef __hasht__
#define __hasht__ 1

#define HASHSIZE 101

#include "types.h"

struct nlist {
  struct nlist *next;
  symbol *sym;
};

unsigned hash(char *);
struct nlist *lookup(char *, struct nlist *table[]);
symbol *intern(char *, struct nlist *table[]);

void free_hasht(struct nlist *table[]);

#endif
