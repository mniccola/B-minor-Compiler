#include "decl.h"

extern void print_tabs(int indent);
extern int typecheck_errors;

struct decl *decl_create(char * name, struct type *type, struct expr *value, struct stmt *code, struct decl *next) {
    struct decl *d  = malloc(sizeof(struct decl));

    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;

    return d;

}

void decl_print(struct decl *d, int indent) {
    if(!d) {
        return;
    }
    print_tabs(indent);
    printf("%s:", d->name);
    type_print(d->type);
    if (!d->value && !d->code) {
        printf(";\n");
    }
    if (d->value) {
        printf(" = ");
        expr_print(d->value);
        printf(";\n");
    }
    if (d->code && d->type->kind == TYPE_FUNCTION) {
        printf(" = ");
        stmt_print(d->code, indent + 1, false);
    }
    decl_print(d->next, indent);
    
}

int decl_resolve(struct decl *d) {
    if(!d) {
        return 0;
    }

    int errors = 0;
    // printf("checking scope level\n");
    // printf("curr level: %d\n", level);
    symbol_t kind = scope_level() > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
    // printf("scope level checked\n");

    d->symbol = symbol_create(kind, type_copy(d->type), d->name);

    errors = errors + expr_resolve(d->value);
    if(scope_lookup_current(d->name)) {
        if(!d->code) {
            printf("resolve error: %s has already been defined in this scope.\n", d->name);
            errors++;
        }
    } else {
        scope_bind(d->name, d->symbol);
    }

    if(d->code) {
        scope_enter();
        errors = errors + param_list_resolve(d->type->params);
        errors = errors + stmt_resolve(d->code);
        scope_exit();
    }

    errors = errors + decl_resolve(d->next);
    return errors;
}

void decl_typecheck(struct decl *d) {
    if(!d) {
        return;
    }

    if(d->value) {
        struct type *t;
        t = expr_typecheck(d->value);
        if(!type_equals(t, d->symbol->type)) {
            printf("type error: assignment must be between two items of the same type: currently between types");
            type_print(d->symbol->type);
            printf("(%s) and", d->symbol->name);
            type_print(t);
            printf(" (");
            expr_print(d->value);
            printf(")\n");
            typecheck_errors++;
        }
    }
    if(d->code) {
        struct param_list *prototype = scope_lookup(d->name)->type->params;
        struct param_list *defined = d->symbol->type->params;
        while(prototype && defined) {
            if(!type_equals(prototype->type, defined->type)) {
                printf("type error: function parameters must have same type as in prototype!\n");
                typecheck_errors++;
            }
            prototype = prototype->next;
            defined = defined->next;
        }
        // printf("Func Name: %s\n", d->name);
        // printf("checking d->code\n");
        stmt_typecheck(d->code, d->name);
    }

    decl_typecheck(d->next);
}