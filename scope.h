#ifndef SCOPE_H
#define SCOPE_H

#include <stdlib.h>
#include "symbol.h" 
#include "hash_table.h"

struct stack_item {
    struct hash_table *table;
    struct stack_item *next;
    int depth;
};


void scope_enter();
void scope_exit();
int scope_level();

void scope_bind(const char *name, struct symbol *sym);
struct symbol *scope_lookup(const char *name);
struct symbol *scope_lookup_current(const char * name);


#endif

