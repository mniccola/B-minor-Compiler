#include "scope.h"


struct stack_item *top = NULL;


void scope_enter() {
    struct stack_item *t = (struct stack_item *)malloc(sizeof(struct stack_item));
    t->table = hash_table_create(0, 0);
    t->next = top;
    top = t;
    top->depth = 0;
}


void scope_exit() {
    if(top != NULL) {
        top = top->next;
    }
}

int scope_level() {
    // printf("top of scope_level\n");
    struct stack_item *curr = top;
    int count = 0;
    while(curr) {
        count++;
        curr = curr->next;
    }
    // return top->depth;
    return count;
}

void scope_bind(const char *name, struct symbol *sym) {
    sym->which = top->depth;
    top->depth++;
    hash_table_insert(top->table, name, sym);
}


struct symbol *scope_lookup(const char *name) {
    struct stack_item *curr = top;
    struct symbol *s;
    while(curr) {
        s = (struct symbol *)hash_table_lookup(curr->table, name);
        if(s) {
            return s;
        }
        curr = curr->next;
    }

    return NULL;
}


struct symbol *scope_lookup_current(const char * name) {
    struct symbol *s = (struct symbol *) hash_table_lookup(top->table, name);
}