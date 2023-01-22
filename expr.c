#include "expr.h"

extern int typecheck_errors;

struct expr * expr_create(expr_t kind, struct expr *left, struct expr *right) {
    struct expr *e = malloc(sizeof(struct expr));
    e->kind = kind;
    e->left = left;
    e->right = right;
    e->arr_literal_next = NULL;

    return e;
}

struct expr * expr_create_name( const char *n ) {
    struct expr *e = malloc(sizeof(struct expr));
    e->kind = EXPR_NAME;
    e->name = n;

    return e;
}

struct expr * expr_create_integer_literal( int c ) {
    struct expr *e = malloc(sizeof(struct expr));
    e->left = NULL;
    e->right = NULL;
    e->literal_value = c;
    e->kind = EXPR_INTEGER_LITERAL;
    return e;
}

struct expr * expr_create_boolean_literal( int c ) {
    struct expr *e  = malloc(sizeof(struct expr));
    e->left = NULL;
    e->right = NULL;
    e->literal_value = c;
    e->kind = EXPR_BOOLEAN_LITERAL;
    return e;
}

struct expr * expr_create_char_literal( char *c ) {
    struct expr *e  = malloc(sizeof(struct expr));
    e->left = NULL;
    e->right = NULL;
    e->string_literal = c;
    e->kind = EXPR_CHAR_LITERAL;
    return e;
}

struct expr * expr_create_string_literal( const char *str ) {
    struct expr *e = malloc(sizeof(struct expr));
    e->left = NULL;
    e->right = NULL;
    e->string_literal = str;
    e->kind = EXPR_STRING_LITERAL;

    return e;
}

struct expr * expr_create_func_call(const char *str, struct expr *param_next) {
    struct expr *e = malloc(sizeof(struct expr));
    e->left = NULL;
    e->right = NULL;
    e->name = str;
    e->kind = EXPR_FUNC_CALL;
    e->func_params = param_next;
    e->reg = -1;

    return e;
}

struct expr * expr_create_arr_index(const char *str, struct expr *indx_list) {
    struct expr *e = malloc(sizeof(struct expr));
    e->name = str;
    e->kind = EXPR_ARR_INDX;
    e->left = NULL;
    e->right = NULL;
    e->indx_next = indx_list;

    return e;
}


struct expr * expr_create_ternary(struct expr *left, struct expr *right, struct expr *next) {
    struct expr *e = malloc(sizeof(struct expr));
    e->kind = EXPR_TERNARY;
    e->left = left;
    e->right = right;
    e->next = next;
}

struct expr * expr_create_array_literal(struct expr *arr_literal_next) {
    struct expr *e = malloc(sizeof(struct expr));
    e->kind = EXPR_ARR_LITERAL;
    e->left = NULL;
    e->right = NULL;
    e->arr_literal_next = arr_literal_next;
    e->next = NULL;
}


bool expr_check_precedence(struct expr *upper, struct expr *lower) {
    if(!upper) {
        return false;
    }
    if(!lower) {
        return false;
    }
    int top = get_precedence(upper);
    int bottom = get_precedence(lower);
    if(top < 1) {
        return false;
    }
    if(bottom < 1) {
        return false;
    }
    if(top >= bottom) {
        return true;
    } else {
        return false;
    }



}

int get_precedence(struct expr *e) {
    if(!e) {
        return -1;
    }
    switch (e->kind) {
        case EXPR_NOT:
            return 10;
            break;
        case EXPR_ADD:
            return 1;
            break;
        case EXPR_SUB:
            return 2;
            break;
        case EXPR_MUL:
            return 3;
            break;
        case EXPR_DIV:
            return 3;
            break;
        case EXPR_MOD:
            return 3;
            break;
        case EXPR_EXPONENT:
            return 3;
            break;
        case EXPR_GT:
            return 5;
            break;
        case EXPR_LT:
            return 5;
            break;
        case EXPR_GE:
            return 5;
            break;
        case EXPR_LE:
            return 5;
            break;
        case EXPR_EQ:
            return 6;
            break;
        case EXPR_NE:
            return 6;
            break;
        case EXPR_AND:
            return 7;
            break;
        case EXPR_OR:
            return 7;
            break;
        case EXPR_TERNARY:
            return 10;
            break;
        case EXPR_ASSIGN:
            return 9;
            break;
        default:
            return -1;
            break;
    }
}



bool is_inList(expr_t val, expr_t arr [], int length) {
    for(int i = 0; i < length; i++) {
        if(arr[i] == val) {
            return true;
        }
    }
    return false;
}

void expr_print(struct expr *e) {
    if(!e) {
        return;
    }
    bool nested = false;
    struct expr *curr;
    if(expr_check_precedence(e, e->left)) {
        printf("( ");
        nested = true;
    }
// if((get_precedence(e) >= get_precedence(e->left) && get_precedence(e) != -1 && get_precedence(e->left) != -1)) {
//         printf("( ");
//         left_nested = true;
//     }
    
    expr_print(e->left);
    if(nested) {
        printf(" )");
        nested = false;
    }
    switch(e->kind) {
        case EXPR_ADD:
            printf(" + ");
            break;
        case EXPR_SUB:
            printf(" - ");
            break;
        case EXPR_MUL:
            printf(" * ");
            break;
        case EXPR_DIV:
            printf(" / ");
            break;
        case EXPR_GT:
            printf(" > ");
            break;
        case EXPR_GE:
            printf(" >= ");
            break;
        case EXPR_LT:
            printf(" < ");
            break;
        case EXPR_LE:  
            printf(" <= ");
            break;
        case EXPR_EQ:
            printf(" == ");
            break;
        case EXPR_NE:
            printf(" != ");
            break;
        case EXPR_MOD:
            printf(" %% ");
            break;
        case EXPR_EXPONENT:
            printf(" ^ ");
            break;
        case EXPR_POST_ADD:
            printf("++");
            break;
        case EXPR_POST_DEC:
            printf("--");
            break;
        case EXPR_NEG:
            printf("-");
            break;
        case EXPR_INTEGER_LITERAL:
            printf("%d", e->literal_value);
            break;
        case EXPR_BOOLEAN_LITERAL:
            if(e->literal_value) {
                printf("true");
            } else {
                printf("false");
            }
            break;
        case EXPR_CHAR_LITERAL:
            printf("%s", e->string_literal);
            break;
        case EXPR_STRING_LITERAL:
            printf("%s", e->string_literal);
            break;
        case EXPR_AND:
            printf(" && ");
            break;
        case EXPR_OR:
            printf(" || ");
            break;
        case EXPR_NOT:
            printf("!");
            break;
        case EXPR_ASSIGN:
            printf(" = ");
            break;
        case EXPR_FUNC_CALL:
            printf("%s(", e->name);
            curr = e->next;
            while(curr) {
                expr_print(curr);
                if(curr->next) {
                    printf(",");
                }
                curr = curr->next;
            }
            printf(")");
            break;
        case EXPR_ARR_INDX:
            printf("%s", e->name);
            curr = e->indx_next;
            while(curr) {
                printf("[");
                expr_print(curr);
                printf("]");
                curr = curr->indx_next;
            }
            break;
        case EXPR_TERNARY:
            expr_print(e->left);
            printf(" ? ");
            expr_print(e->right);
            printf(" : ");
            expr_print(e->next);
            break;
        case EXPR_ARR_LITERAL:
            curr = e->arr_literal_next;
            printf("{");
            while(curr) {
                expr_print(curr);
                if(curr->next) {
                    printf(", ");
                }
                curr = curr->next;
            }
            printf("}");
            break;
        case EXPR_NAME:
            printf("%s", e->name);
            break;
        case EXPR_PRINT_LIST:
            curr = e->next;
            while(curr) {
                expr_print(curr);
                if(curr->next) {
                    printf(", ");
                }
                curr = curr->next;
            }
            break;
    }
    if(expr_check_precedence(e, e->right)) {
        printf("( ");
        nested = true;
    }
    // if((get_precedence(e) >= get_precedence(e->right) && get_precedence(e) != -1 && get_precedence(e->right) != -1)) {
    //     right_nested = true;
    //     printf("( ");
    // }
    expr_print(e->right);
    if(nested) {
        printf(" )");
        nested = false;
    }
    // if(is_nested) {
    //     printf(" )");
    // }
    return;
}



int expr_resolve(struct expr *e) {
    // printf("expr_resolve\n");
    if(!e) {
        return 0;
    }

    int errors = 0;
    if(e->kind == EXPR_NAME || e->kind == EXPR_FUNC_CALL) {
        e->symbol = scope_lookup(e->name);
        if(!e->symbol) {
            printf("resolve error: %s is not defined\n", e->name);
            errors++;
        } else {
            print_resolution(e->symbol);
        }
        if(e->kind == EXPR_FUNC_CALL) {
            struct expr * curr_p = e->func_params;
            while(curr_p) {
                errors = errors + expr_resolve(curr_p);
                curr_p = curr_p->next;
            }

        }
    } else if( e->kind == EXPR_ARR_INDX) {
        e->symbol = scope_lookup(e->name);
        if(!e->symbol) {
            printf("resolve error: %s is not defined\n", e->name);
            errors++;
        } else {
            print_resolution(e->symbol);
        }
        struct expr *curr = e->indx_next;
        while(curr) {
            errors = errors + expr_resolve(curr);
            curr = curr->indx_next;
        }
    } else {
        errors = errors + expr_resolve(e->left);
        errors = errors + expr_resolve(e->right);
    }

    return errors;
}


struct type *expr_typecheck(struct expr *e) {
    if(!e) {
        return NULL;
    }
    struct type *lt = expr_typecheck(e->left);
    struct type *rt = expr_typecheck(e->right);

    struct type *result;
    switch(e->kind) {
        case EXPR_INTEGER_LITERAL:
            result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
            break;
        case EXPR_CHAR_LITERAL:
            result = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
            break;
        case EXPR_STRING_LITERAL:
            result = type_create(TYPE_STRING, NULL, NULL, NULL);
            break;
        case EXPR_BOOLEAN_LITERAL:
            result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_NAME:
            result = type_create(e->symbol->type->kind, e->symbol->type->subtype, NULL, NULL);
            break;
        case EXPR_ARR_INDX:;
            struct symbol *sym = scope_lookup(e->name);
            if(sym->type->kind == TYPE_ARRAY) {
                struct type *index = expr_typecheck(e->indx_next);
                if(index->kind != TYPE_INTEGER) {
                    printf("type error: placeholder message ... MUST INDEX ARRAY WITH INTEGER\n");
                    typecheck_errors++;
                }
                result = type_copy(sym->type->subtype);
            } else {
                result = type_copy(sym->type);
            }
            break;
        case EXPR_ADD:
            if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
                printf("type error: cannot add types");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
            break;
        case EXPR_SUB:
            if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
                printf("type error: cannot subtract types");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
            break;
        case EXPR_MUL:
            if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
                printf("type error: cannot multiply types");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
            break;
        case EXPR_DIV:
            if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
                printf("type error: cannot divide types");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
            break;
        case EXPR_MOD:
            if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
                printf("type error: cannot perform modulo operation on types");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
            break;
        case EXPR_EXPONENT:
            if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
                printf("type error: cannot perform exponentiation on");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
            break;
        case EXPR_POST_ADD:
            if(lt->kind != TYPE_INTEGER) {
                printf("type error: cannot post-fix increment type");
                print_type_error(lt, NULL, e);
                typecheck_errors++;
            }
            result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
            break;
        case EXPR_POST_DEC:
            if(lt->kind != TYPE_INTEGER) {
                printf("type error: cannot post-fix decrement type");
                print_type_error(lt, NULL, e);
                typecheck_errors++;
            }
            result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
            break;
        case EXPR_NEG:
            if(rt->kind != TYPE_INTEGER) {
                printf("type error: cannot negate a type");
                switch (rt->kind) {
                        case TYPE_STRING:
                            printf("string (");
                            expr_print(e->right);
                            printf(")\n");
                            break;
                        case TYPE_CHARACTER:
                            printf("character (");
                            expr_print(e->left);
                            printf(")\n");
                            break;
                        case TYPE_INTEGER:
                            printf("integer (");
                            expr_print(e->left);
                            printf(")\n");
                        case TYPE_BOOLEAN:
                            printf("boolean (");
                            expr_print(e->left);
                            printf(")\n");
                            break;
                        case TYPE_ARRAY:
                            printf("array (");
                            expr_print(e->left);
                            printf(")\n");
                            break;
                    }
                    typecheck_errors++;
            }
            result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
            break;
        case EXPR_GT:
            if(!type_equals(lt, rt)) {
                printf("cannot comapare ( > ) types");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
                printf("type error: placeholder message ... COMPARISON ERROR! GT\n");
                typecheck_errors++;
            }
            if(rt->kind == TYPE_VOID || rt->kind == TYPE_ARRAY || rt->kind == TYPE_FUNCTION) {
                printf("type error: placeholder message ... COMPARISON ERROR! GT\n");
                typecheck_errors++;
            }
            result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_GE:
            if(!type_equals(lt, rt)) {
                    printf("cannot comapare ( >= ) types ");
                    print_type_error(lt, rt, e);
                    typecheck_errors++;
                }
            if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
                printf("type error: placeholder message ... COMPARISON ERROR! GE\n");
                typecheck_errors++;
            }
            if(rt->kind == TYPE_VOID || rt->kind == TYPE_ARRAY || rt->kind == TYPE_FUNCTION) {
                printf("type error: placeholder message ... COMPARISON ERROR! GE\n");
                typecheck_errors++;
            }
            result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_LT:
            if(!type_equals(lt, rt)) {
                printf("cannot comapare ( < ) tyeps ");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
                printf("type error: placeholder message ... COMPARISON ERROR! LT\n");
                typecheck_errors++;
            }
            if(rt->kind == TYPE_VOID || rt->kind == TYPE_ARRAY || rt->kind == TYPE_FUNCTION) {
                printf("type error: placeholder message ... COMPARISON ERROR! LT\n");
                typecheck_errors++;
            }
            result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_LE:
            if(!type_equals(lt, rt)) {
                printf("cannot comapare ( <= ) types ");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
                printf("type error: placeholder message ... COMPARISON ERROR! LE\n");
                typecheck_errors++;
            }
            if(rt->kind == TYPE_VOID || rt->kind == TYPE_ARRAY || rt->kind == TYPE_FUNCTION) {
                printf("type error: placeholder message ... COMPARISON ERROR! LE\n");
                typecheck_errors++;
            }
            result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_EQ:
            if(!type_equals(lt, rt)) {
                printf("cannot comapare ( == ) types ");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
                printf("type error: placeholder message ... COMPARISON ERROR! EQ\n");
                typecheck_errors++;
            }
            if(rt->kind == TYPE_VOID || rt->kind == TYPE_ARRAY || rt->kind == TYPE_FUNCTION) {
                printf("type error: placeholder message ... COMPARISON ERROR! EQ\n");
                typecheck_errors++;
            }
            result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_NE:
            if(!type_equals(lt, rt)) {
                printf("cannot comapare ( != ) types ");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
                printf("type error: placeholder message ... COMPARISON ERROR! NE\n");
                typecheck_errors++;
            }
            if(rt->kind == TYPE_VOID || rt->kind == TYPE_ARRAY || rt->kind == TYPE_FUNCTION) {
                printf("type error: placeholder message ... COMPARISON ERROR! NE\n");
                typecheck_errors++;
            }
            result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_AND:
            if(lt->kind != TYPE_BOOLEAN || rt->kind != TYPE_BOOLEAN) {
                printf("boolean operations ( && ) must compare items of type boolean. not types");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_OR:
            if(lt->kind != TYPE_BOOLEAN || rt->kind != TYPE_BOOLEAN) {
                printf("boolean operations ( || ) must compare items of type boolean. not types");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_NOT:
            if(lt->kind != TYPE_BOOLEAN || rt->kind != TYPE_BOOLEAN) {
                printf("type error: logical not ( ! ) must operate on items of type boolean not types");
                typecheck_errors++;
            }
            result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_ASSIGN:
            if(!type_equals(lt, rt)) {
                printf("type error: assignment must be between two items of the same type: currently between types");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            result = type_copy(lt);
            break;
        case EXPR_TERNARY:
            if(lt->kind != TYPE_BOOLEAN) {
                printf("type error: ternary condition must evaluate to boolean value: currently:");
                print_type_error(lt, NULL, e);
                typecheck_errors++;
            }
            struct type *next = expr_typecheck(e->next);
            if(!type_equals(rt, next)) {
                printf("type error: ternary option types must match: currently:");
                print_type_error(lt, rt, e);
                typecheck_errors++;
            }
            result = type_copy(rt);
            break;
        case EXPR_FUNC_CALL:;
            struct expr *params = e->func_params;
            struct symbol *s = scope_lookup(e->name);
            struct param_list *def_list = s->type->params;
            struct type *expr_type;

            int declared_count = 0;
            int count = 0;

            // count number of defined parameters
            while(def_list) {
                def_list = def_list->next;
                declared_count++;
            }

            def_list = s->type->params;

            while(params && def_list) {
                expr_type = expr_typecheck(params);
                if(!type_equals(expr_type, def_list->type)) {
                    printf("type error: placeholder message ... FUNCTION PARAMETER ERROR!\n");
                    typecheck_errors++;
                }
                params = params->next;
                def_list = def_list->next;
                count++;
            }
            // check if num parameters is the same.
            if(count != declared_count) {
                printf("type error: placeholder message ... INCORRECT NUMBER OF FUNCTION PARAMETERS!\n");
                typecheck_errors++;
            }

            result = type_create(s->type->subtype->kind, NULL, param_list_copy(s->type->params), NULL);
            break;
        case EXPR_ARR_LITERAL:;
            struct expr *curr = e->arr_literal_next;
            struct type *first_type = expr_typecheck(curr);
            bool same_type = true;
            struct type *curr_type;
            while(curr) {
                curr_type = expr_typecheck(curr);
                if(!type_equals(first_type, curr_type)) {
                    printf("type error: placeholder message ... ARRAY LITERAL ERROR!\n");
                    typecheck_errors++;
                }
                curr = curr->next;
            }
            result = type_create(TYPE_ARRAY, type_create(first_type->kind, NULL, NULL, NULL), NULL, NULL);
            break;
        case EXPR_PRINT_LIST:;
            struct expr *curr_print = e->next;
            struct type *curr_print_type;
            int print_count = 0;
            while(curr_print) {
                curr_type = expr_typecheck(curr_print);
                curr_print = curr_print->next;
                print_count++;
            }
            break;
    }

    return result;
}

