#include "type.h"

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params, struct expr* array_length) {
    struct type *t  = malloc(sizeof(struct type));
    t->kind = kind;
    t->subtype = subtype;
    t->params = params;
    t->arr_length = array_length;

    return t;
}


void type_print(struct type *t) {
    if(!t) {
        return;
    }

    switch(t->kind) {
        case TYPE_STRING:
            printf(" string");
            break;
        case TYPE_VOID:
            printf(" void");
            break;
        case TYPE_CHARACTER:
            printf(" char");
            break;
        case TYPE_BOOLEAN:
            printf(" boolean");
            break;
        case TYPE_INTEGER:
            printf(" integer");
            break;
        case TYPE_ARRAY:
            printf(" array");
            printf(" [");
            expr_print(t->arr_length);
            printf("]");
            type_print(t->subtype);
            break;
        case TYPE_FUNCTION:
            printf(" function");
            type_print(t->subtype);
            printf("(");
            param_list_print(t->params);
            printf(")");
            break;
        case TYPE_AUTO:
            printf(" auto");
            break;
    }
}

bool type_equals(struct type *a, struct type *b) {
    if(!a && !b) {
        return true;
    }
    if(!a || !b) {
        return false;
    }

    if(a->kind == b->kind) {
        if(a->kind == TYPE_ARRAY) {
            return type_equals(a->subtype, b->subtype);
        } else if(a->kind == TYPE_FUNCTION) {
            bool sub = type_equals(a->subtype, b->subtype);
            bool param = param_list_equals(a->params, b->params);
            return sub && param;
        } else {
            return true;
        }

    // } else if(a->kind == TYPE_FUNCTION) {
    //     return type_equals(a->subtype, b);
    // } else if(b->kind == TYPE_FUNCTION) {
    //     return type_equals(a, b->subtype);
    } else {
        return false; 
    }
}

struct type *type_copy(struct type *t) {
    if(!t) {
        return NULL;
    }
    return type_create(t->kind, type_copy(t->subtype), param_list_copy(t->params), NULL);
}

void type_delete(struct type *t) {
    if(!t) {
        return;
    }
    // param_list_delete(t->params)
    type_delete(t->subtype);
    free(t->arr_length);
}