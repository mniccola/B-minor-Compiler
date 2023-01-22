#include "stmt.h"

extern void print_tabs(int indent);
extern int typecheck_errors;

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next ) {
    struct stmt *s  = malloc(sizeof(struct stmt));

    s->kind = kind;
    s->decl = decl;
    s->init_expr = init_expr;
    s->expr = expr;
    s->next_expr = next_expr;
    s->body = body;
    s->else_body = else_body;
    s->next = next;

    return s;
}



void stmt_print(struct stmt *s, int indent, bool else_block) {
    if(!s) {
        return;
    } 
    bool else_format = false;
    switch(s->kind) {
        case STMT_DECL:
            decl_print(s->decl, indent);
            break;
        case STMT_EXPR:
            print_tabs(indent);
            expr_print(s->expr);
            printf(";\n");
            break;
        case STMT_IF_ELSE:
            print_tabs(indent);
            printf("if(");
            expr_print(s->expr);
            printf(") ");
            if(s->else_body) {
                else_format = true;
            }
            if(s->body->kind == STMT_BLOCK) {
                stmt_print(s->body, indent + 1, else_format);
            } else {
                printf("\n");
                stmt_print(s->body, indent + 1, else_format);
            }
            else_format = false;
            if(s->else_body) {
                if(s->else_body->kind == STMT_BLOCK) {
                    printf(" else ");
                    stmt_print(s->else_body, indent + 1, else_format);
                } else {
                    print_tabs(indent);
                    printf("else\n");
                    stmt_print(s->else_body, indent + 1, else_format);
                }   
            }
            break;
        case STMT_BLOCK:
            if(else_block) {
                printf("{\n");
                stmt_print(s->body, indent, else_format);
                print_tabs(indent - 1);
                printf("}");
            } else {
                printf("{\n");
                stmt_print(s->body, indent, else_format);
                print_tabs(indent - 1);
                printf("}\n");
            }
            break;
        case STMT_FOR:
            print_tabs(indent);
            printf("for(");
            expr_print(s->init_expr);
            printf(";");
            expr_print(s->expr);
            printf(";");
            expr_print(s->next_expr);
            printf(")");
            if(s->body->kind == STMT_BLOCK) {
                stmt_print(s->body, indent + 1, else_format);
            } else {
                printf("\n");
                stmt_print(s->body, indent + 1, else_format);
            }
            break;
        case STMT_PRINT:
            print_tabs(indent);
            printf("print ");
            expr_print(s->expr);
            printf(";\n");
            break;
        case STMT_RETURN:
            print_tabs(indent);
            printf("return ");
            expr_print(s->expr);
            printf(";\n");
            break;
    }
    stmt_print(s->next, indent, false);
}


int stmt_resolve(struct stmt *s) {
    if(!s) {
        return 0;
    }
    int errors = 0;
    struct expr *curr;
    switch(s->kind) {
        case STMT_BLOCK:
            scope_enter();
            errors = errors + stmt_resolve(s->body);
            scope_exit();
            break;
        case STMT_FOR:
            errors = errors + expr_resolve(s->init_expr);
            errors = errors + expr_resolve(s->expr);
            errors = errors + expr_resolve(s->next_expr);
            errors = errors + stmt_resolve(s->body);
            break;
        case STMT_EXPR:
            errors = errors + expr_resolve(s->expr);
            break;
        case STMT_DECL:
            // printf("resolving stmt_decl\n");
            errors = errors + decl_resolve(s->decl);
            break;
        case STMT_PRINT:
            curr = s->expr;
            while(curr) {
                errors = errors + expr_resolve(curr);
                curr = curr->next;
            }
            break;
        case STMT_RETURN:
            // printf("resolving return\n");
            errors = errors + expr_resolve(s->expr);
            break;
        case STMT_IF_ELSE:
            errors = errors + expr_resolve(s->expr);
            errors = errors + stmt_resolve(s->body);
            errors = errors + stmt_resolve(s->else_body);
            break;
    }

    errors = errors + stmt_resolve(s->next);

    return errors;

}


void stmt_typecheck(struct stmt *s, char *func_name) {
    if(!s) {
        return;
    }
    struct type *t;
    switch(s->kind) {
        case STMT_EXPR:
            t = expr_typecheck(s->expr);
            break;
        case STMT_IF_ELSE:
            t = expr_typecheck(s->expr);
            if(t->kind != TYPE_BOOLEAN) {
                printf("type error: non boolean statement in if else\n");
            }
            stmt_typecheck(s->body, func_name);
            stmt_typecheck(s->else_body, func_name);
            break;
        case STMT_DECL:
            // printf("stmt_decl\n");
            decl_typecheck(s->decl);
            break;
        case STMT_FOR:
            t = expr_typecheck(s->init_expr);
            if(t) {
                if(t->kind != TYPE_INTEGER) {
                    printf("type error: first argument of for loop is not an integer\n");
                }
            }
            // break;
            t = expr_typecheck(s->expr);
            if(t) {
                if(t->kind != TYPE_BOOLEAN) {
                    printf("type error: for loop condition must be of type integer\n");
                }
            }
            // break;
            t = expr_typecheck(s->next_expr);
            if(t) {
                if(t->kind != TYPE_INTEGER) {
                    printf("type error: for loop increment must be of type integer\n");
                }
            }
            stmt_typecheck(s->body, func_name);
            break;
        case STMT_PRINT:;
            // printf("stmt_print\n");
            // struct expr *curr = s->expr;
            t = expr_typecheck(s->expr);
            //int count = 0;
            // while(curr) {
            //     printf("printing... %d\n", count);
            //     printf("%s\n", curr->name);
            //     t = expr_typecheck(curr);
            //     curr = curr->next;
            //     count++;
            // }
            break;
        case STMT_BLOCK:;
            stmt_typecheck(s->body, func_name);
            break;
        case STMT_RETURN:
            t = expr_typecheck(s->expr);
            struct symbol *sym = scope_lookup(func_name);
            if(!type_equals(t, sym->type->subtype)) {
                printf("type error: value returned must have the same type as function declaration\n");
                typecheck_errors++;
            }
            break;

    }

    stmt_typecheck(s->next, func_name);
}