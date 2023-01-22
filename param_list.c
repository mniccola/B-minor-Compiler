#include "param_list.h"

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next ) {
    struct param_list *p  = malloc(sizeof(struct param_list));
    p->name = strdup(name);
    p->type = type;
    p->next = next;

    return p;
}

void param_list_print(struct param_list *a) {
    if(!a) {
        return;
    }

    printf("%s:", a->name);
    type_print(a->type);
    if(a->next) {
        printf(", ");
        param_list_print(a->next);
    }
}

int param_list_resolve(struct param_list *p) {
    struct param_list *curr = p;
    int errors = 0;
    while(p) {
        // printf("param->type: %d\n", p->type->kind);
        p->symbol = symbol_create(SYMBOL_PARAM, p->type, p->name);
        if(scope_lookup_current(p->name)) {
             printf("resolve error: %s has already been defined in this scope.\n", p->name);
             errors++;
        } else {
            scope_bind(p->name, p->symbol);
        }
        p = p->next;
    }
    return errors;
}

bool param_list_equals(struct param_list *a, struct param_list *b) {
    if(!a && !b) {
        return true;
    }
    if(!a || !b) {
        return false;
    }
    if(a->type->kind, b->type->kind) {
        if(a->type->kind == TYPE_ARRAY) {
            if(!type_equals(a->type->subtype, b->type->subtype)) {
                return false;
            } else {
                return param_list_equals(a->next, b->next);
            }
        } else if(a->type->kind != TYPE_FUNCTION) {
            return param_list_equals(a->next, b->next);
        }
    } else {
        return false;
    }

}

struct param_list * param_list_copy(struct param_list *p) {
    if(!p) {
        return NULL;
    }

    return param_list_create(p->name, type_copy(p->type), param_list_copy(p->next));
}
