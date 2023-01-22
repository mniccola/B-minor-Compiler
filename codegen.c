#include "codegen.h"

extern FILE * output_file;

int curr_local_count = 0;
int curr_param_count = 0;
int loop_labels = 0;
int return_count = 0;

void expr_codegen(struct expr *e) {
    if(!e) {
        return;
    }

    switch(e->kind) {
        case EXPR_NAME:
            e->reg = scratch_alloc();
            fprintf(output_file, "MOVQ %s, %s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
            break;
        case EXPR_INTEGER_LITERAL:
            e->reg = scratch_alloc();
            fprintf(output_file, "MOVQ $%d, %s\n", e->literal_value, 
                scratch_name(e->reg));
            break;
        case EXPR_BOOLEAN_LITERAL:
            e->reg = scratch_alloc();
            fprintf(output_file, "MOVQ $%d, %s\n", e->literal_value,
                scratch_name(e->reg));
            break;
        case EXPR_CHAR_LITERAL:
            e->reg = scratch_alloc();
            fprintf(output_file, "MOVQ $%d, %s\n", e->string_literal[1], 
                scratch_name(e->reg));
            break;
        case EXPR_STRING_LITERAL:
            e->reg = scratch_alloc();
            int str_label = label_create();
            fprintf(output_file, ".data\n");
            fprintf(output_file, "%s:\n", label_name(str_label));
            fprintf(output_file, ".string %s\n", e->string_literal);
            fprintf(output_file, ".text\n");
            fprintf(output_file, "LEAQ %s, %s\n", label_name(str_label), scratch_name(e->reg));
            break;
        case EXPR_ADD:
            expr_codegen(e->left);
            expr_codegen(e->right);
            fprintf(output_file, "ADDQ %s, %s\n", 
                scratch_name(e->left->reg),
                scratch_name(e->right->reg));
            e->reg = e->right->reg;
            scratch_free(e->left->reg);
            e->return_kind = EXPR_INTEGER_LITERAL;
            break;
        case EXPR_ASSIGN:
            expr_codegen(e->right);
            fprintf(output_file, "MOVQ %s, %s\n",
                scratch_name(e->right->reg),
                symbol_codegen(e->left->symbol));
            e->reg = e->left->reg;
            scratch_free(e->left->reg);
            scratch_free(e->right->reg);
            break;
        case EXPR_SUB:
            expr_codegen(e->left);
            expr_codegen(e->right);

            fprintf(output_file, "SUBQ %s, %s\n", 
                scratch_name(e->right->reg),
                scratch_name(e->left->reg));
            e->reg = e->left->reg;
            scratch_free(e->right->reg);
            e->return_kind = EXPR_INTEGER_LITERAL;
            break;
        case EXPR_MUL:
            expr_codegen(e->left);
            expr_codegen(e->right);
            fprintf(output_file, "MOVQ %s, %%rax\n", scratch_name(e->left->reg));
            fprintf(output_file, "IMULQ %s\n",
                scratch_name(e->right->reg));
            fprintf(output_file, "MOVQ %%rax, %s\n", 
                scratch_name(e->left->reg));
            e->reg = e->left->reg;
            scratch_free(e->right->reg);
            e->return_kind = EXPR_INTEGER_LITERAL;
            break;
        case EXPR_DIV:
            expr_codegen(e->left);
            expr_codegen(e->right);
            fprintf(output_file, "MOVQ %s, %%rax\n", 
                scratch_name(e->left->reg));
            fprintf(output_file, "CQO\n");
            fprintf(output_file, "IDIVQ %s\n",
                scratch_name(e->right->reg));
            fprintf(output_file, "MOVQ %%rax, %s\n",
                scratch_name(e->left->reg));
            e->reg = e->left->reg;
            scratch_free(e->right->reg);
            e->return_kind = EXPR_INTEGER_LITERAL;
            break;
        case EXPR_MOD:
            expr_codegen(e->left);
            expr_codegen(e->right);
            fprintf(output_file, "MOVQ %s, %%rax\n", 
                scratch_name(e->left->reg));
            fprintf(output_file, "CQO\n");
            fprintf(output_file, "IDIVQ %s\n",
                scratch_name(e->right->reg));
            fprintf(output_file, "MOVQ %%rdx, %s\n",
                scratch_name(e->left->reg));
            e->reg = e->left->reg;
            scratch_free(e->right->reg);
            e->return_kind = EXPR_INTEGER_LITERAL;
            break;
        case EXPR_POST_ADD:
            expr_codegen(e->left);
            fprintf(output_file, "MOVQ %s, %%rax\n", 
                scratch_name(e->left->reg));
            fprintf(output_file, "INCQ %%rax\n");
            int ret_val = scratch_alloc();
            fprintf(output_file, "MOVQ %s, %s\n", scratch_name(e->left->reg), scratch_name(ret_val));
            if(e->left->name) {
                fprintf(output_file, "MOVQ %rax, %s\n", symbol_codegen(e->left->symbol));
                e->reg = ret_val;
            } else {
                fprintf(output_file, "MOVQ %%rax, %s\n",
                    scratch_name(e->left->reg));
            }
            scratch_free(e->left->reg);
            e->reg = ret_val;
            e->return_kind = EXPR_INTEGER_LITERAL;
            break;
        case EXPR_POST_DEC:
            expr_codegen(e->left);
            fprintf(output_file, "MOVQ %s, %%rax\n", 
                scratch_name(e->left->reg));
            fprintf(output_file, "DECQ %%rax\n");
            ret_val = scratch_alloc();
            fprintf(output_file, "MOVQ %s, %s\n", scratch_name(e->left->reg), scratch_name(ret_val));
            if(e->left->name) {
                printf("e has name\n");
                fprintf(output_file, "MOVQ %rax, %s\n", symbol_codegen(e->left->symbol));
                e->reg = ret_val;
            } else {
                fprintf(output_file, "MOVQ %%rax, %s\n",
                    scratch_name(e->left->reg));
            }
            scratch_free(e->left->reg);
            e->reg = ret_val;
            e->return_kind = EXPR_INTEGER_LITERAL;
            break;
        case EXPR_EXPONENT:
        expr_codegen(e->left);
        expr_codegen(e->right);
        fprintf(output_file, "MOVQ %s, %%rdi\n", scratch_name(e->left->reg));
        fprintf(output_file, "MOVQ %s, %%rsi\n", scratch_name(e->right->reg));
        fprintf(output_file, "PUSHQ %%rbx\n");
        fprintf(output_file, "PUSHQ %%r10\n");
        fprintf(output_file, "PUSHQ %%r11\n");
        fprintf(output_file, "CALL integer_power\n");
        fprintf(output_file, "POPQ %%r11\n");
        fprintf(output_file, "POPQ %%r10\n");
        fprintf(output_file, "POPQ %%rbx\n");
        fprintf(output_file, "MOVQ %%rax, %s\n", scratch_name(e->left->reg));
        scratch_free(e->right->reg);
        e->reg = e->left->reg;
        e->return_kind = EXPR_INTEGER_LITERAL;
        break;
    case EXPR_GT:
        expr_codegen(e->left);
        expr_codegen(e->right);
        e->reg = scratch_alloc();
        int true_label = label_create();
        int done_label = label_create();
        fprintf(output_file, "CMP %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
        fprintf(output_file, "JG %s\n", label_name(true_label));
        fprintf(output_file, "MOV $0, %s\n", scratch_name(e->reg));
        fprintf(output_file, "JMP %s\n", label_name(done_label));
        fprintf(output_file, "%s:\n", label_name(true_label));
        fprintf(output_file, "MOV $1, %s\n", scratch_name(e->reg));
        fprintf(output_file, "%s:\n", label_name(done_label));
        scratch_free(e->left->reg);
        scratch_free(e->right->reg);
        e->return_kind = EXPR_BOOLEAN_LITERAL;
        break;
    case EXPR_GE:
        expr_codegen(e->left);
        expr_codegen(e->right);
        e->reg = scratch_alloc();
        true_label = label_create();
        done_label = label_create();
        fprintf(output_file, "CMP %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
        fprintf(output_file, "JGE %s\n", label_name(true_label));
        fprintf(output_file, "MOV $0, %s\n", scratch_name(e->reg));
        fprintf(output_file, "JMP %s\n", label_name(done_label));
        fprintf(output_file, "%s:\n", label_name(true_label));
        fprintf(output_file, "MOV $1, %s\n", scratch_name(e->reg));
        fprintf(output_file, "%s:\n", label_name(done_label));
        scratch_free(e->left->reg);
        scratch_free(e->right->reg);
        e->return_kind = EXPR_BOOLEAN_LITERAL;
        break;
    case EXPR_LT:
        expr_codegen(e->left);
        expr_codegen(e->right);
        e->reg = scratch_alloc();
        true_label = label_create();
        done_label = label_create();
        fprintf(output_file, "CMP %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
        fprintf(output_file, "JL %s\n", label_name(true_label));
        fprintf(output_file, "MOV $0, %s\n", scratch_name(e->reg));
        fprintf(output_file, "JMP %s\n", label_name(done_label));
        fprintf(output_file, "%s:\n", label_name(true_label));
        fprintf(output_file, "MOV $1, %s\n", scratch_name(e->reg));
        fprintf(output_file, "%s:\n", label_name(done_label));
        scratch_free(e->left->reg);
        scratch_free(e->right->reg);
        e->return_kind = EXPR_BOOLEAN_LITERAL;
        break;
    case EXPR_LE:   
        expr_codegen(e->left);
        expr_codegen(e->right);
        e->reg = scratch_alloc();
        true_label = label_create();
        done_label = label_create();
        fprintf(output_file, "CMP %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
        fprintf(output_file, "JLE %s\n", label_name(true_label));
        fprintf(output_file, "MOV $0, %s\n", scratch_name(e->reg));
        fprintf(output_file, "JMP %s\n", label_name(done_label));
        fprintf(output_file, "%s:\n", label_name(true_label));
        fprintf(output_file, "MOV $1, %s\n", scratch_name(e->reg));
        fprintf(output_file, "%s:\n", label_name(done_label));
        scratch_free(e->left->reg);
        scratch_free(e->right->reg);
        e->return_kind = EXPR_BOOLEAN_LITERAL;
        break;
    case EXPR_EQ:
        expr_codegen(e->left);
        expr_codegen(e->right);
        e->reg = scratch_alloc();
        true_label = label_create();
        done_label = label_create();
        fprintf(output_file, "CMP %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
        fprintf(output_file, "JE %s\n", label_name(true_label));
        fprintf(output_file, "MOV $0, %s\n", scratch_name(e->reg));
        fprintf(output_file, "JMP %s\n", label_name(done_label));
        fprintf(output_file, "%s:\n", label_name(true_label));
        fprintf(output_file, "MOV $1, %s\n", scratch_name(e->reg));
        fprintf(output_file, "%s:\n", label_name(done_label));
        scratch_free(e->left->reg);
        scratch_free(e->right->reg);
        e->return_kind = EXPR_BOOLEAN_LITERAL;
        break;
    case EXPR_NE:
        expr_codegen(e->left);
        expr_codegen(e->right);
        e->reg = scratch_alloc();
        true_label = label_create();
        done_label = label_create();
        fprintf(output_file, "CMP %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
        fprintf(output_file, "JNE %s\n", label_name(true_label));
        fprintf(output_file, "MOV $0, %s\n", scratch_name(e->reg));
        fprintf(output_file, "JMP %s\n", label_name(done_label));
        fprintf(output_file, "%s:\n", label_name(true_label));
        fprintf(output_file, "MOV $1, %s\n", scratch_name(e->reg));
        fprintf(output_file, "%s:\n", label_name(done_label));
        scratch_free(e->left->reg);
        scratch_free(e->right->reg);
        e->return_kind = EXPR_BOOLEAN_LITERAL;
        break;
    case EXPR_AND:
        expr_codegen(e->left);
        expr_codegen(e->right);
        e->reg = scratch_alloc();
        true_label = label_create();
        done_label = label_create();
        fprintf(output_file, "AND %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
        fprintf(output_file, "CMP $0, %s\n", scratch_name(e->right->reg));
        fprintf(output_file, "JNE %s\n", label_name(true_label));
        fprintf(output_file, "MOV $0, %s\n", scratch_name(e->reg));
        fprintf(output_file, "JMP %s\n", label_name(done_label));
        fprintf(output_file, "%s:\n", label_name(true_label));
        fprintf(output_file, "MOV $1, %s\n", scratch_name(e->reg));
        fprintf(output_file, "%s:\n", label_name(done_label));
        scratch_free(e->left->reg);
        scratch_free(e->right->reg);
        e->return_kind = EXPR_BOOLEAN_LITERAL;
        break;
    case EXPR_OR:
        expr_codegen(e->left);
        expr_codegen(e->right);
        e->reg = scratch_alloc();
        true_label = label_create();
        done_label = label_create();
        fprintf(output_file, "OR %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
        fprintf(output_file, "CMP $0, %s\n", scratch_name(e->right->reg));
        fprintf(output_file, "JNE %s\n", label_name(true_label));
        fprintf(output_file, "MOV $0, %s\n", scratch_name(e->reg));
        fprintf(output_file, "JMP %s\n", label_name(done_label));
        fprintf(output_file, "%s:\n", label_name(true_label));
        fprintf(output_file, "MOV $1, %s\n", scratch_name(e->reg));
        fprintf(output_file, "%s:\n", label_name(done_label));
        scratch_free(e->left->reg);
        scratch_free(e->right->reg);
        e->return_kind = EXPR_BOOLEAN_LITERAL;
        break;
    case EXPR_NOT:
        break;
    case EXPR_NEG:
        expr_codegen(e->right);
        fprintf(output_file, "MOVQ $%d, %%rax\n", -1);
            fprintf(output_file, "IMULQ %s\n",
                scratch_name(e->right->reg));
            fprintf(output_file, "MOVQ %%rax, %s\n", 
                scratch_name(e->right->reg));
            e->reg = e->right->reg;
            e->return_kind = EXPR_INTEGER_LITERAL;
            break;
    case EXPR_FUNC_CALL:;
        e->reg = scratch_alloc();
        struct expr *curr = e->func_params;

        // look at each arg before we assign
        // to deal with func calls as args
        while (curr) {
            expr_codegen(curr);
            curr = curr->next;
        }

        // move each argument into argument registers using if gauntlet.
        curr = e->func_params;

        if (curr) {
            fprintf(output_file, "MOVQ %s, %%rdi\n", scratch_name(curr->reg));
            scratch_free(curr->reg);
            curr = curr->next;
        }


        if (curr) {
        fprintf(output_file, "MOVQ %s, %%rsi\n", scratch_name(curr->reg));
        scratch_free(curr->reg);
        curr = curr->next;
        }

        if (curr) {
            fprintf(output_file, "MOVQ %s, %%rdx\n", scratch_name(curr->reg));
            scratch_free(curr->reg);
            curr = curr->next;
        }

        if (curr) {
            fprintf(output_file, "MOVQ %s, %%rcx\n", scratch_name(curr->reg));
            scratch_free(curr->reg);
            curr = curr->next;
        }

        if (curr) {
            fprintf(output_file, "MOVQ %s, %%r8\n", scratch_name(curr->reg));
            scratch_free(curr->reg);
            curr = curr->next;
        }

        if (curr) {
            fprintf(output_file, "MOVQ %s, %%r9\n", scratch_name(curr->reg));
            scratch_free(curr->reg);
            curr = curr->next;
        }

        if (curr) {
            printf("codegen error: too many arguments\n");
            exit(1);
        }
        // push callee regs
        fprintf(output_file, "PUSHQ %%rbx\n");
        fprintf(output_file, "PUSHQ %%r10\n");
        fprintf(output_file, "PUSHQ %%r11\n");

        
        fprintf(output_file, "CALL %s\n", e->name);

        // pop callee regs
        fprintf(output_file, "POPQ %%r11\n");
        fprintf(output_file, "POPQ %%r10\n");
        fprintf(output_file, "POPQ %%rbx\n");
        
        fprintf(output_file, "MOVQ %%rax, %s\n", scratch_name(e->reg));
    }

}



void decl_codegen(struct decl *d) {
    // printf("top of decl_codegen\n");
    if(!d) {
        return;
    }

    if(!d->code) {
        if(d->symbol->type->kind != TYPE_FUNCTION) {
            if(d->symbol->kind == SYMBOL_GLOBAL) {
                fprintf(output_file, ".data\n");
                fprintf(output_file, ".global %s\n", d->symbol->name);
                fprintf(output_file, "%s:\n", d->symbol->name);
                if(d->value) {
                    switch(d->value->kind) {
                        case EXPR_INTEGER_LITERAL:
                            fprintf(output_file, "\t.quad %d\n", d->value->literal_value);
                            break;
                        case EXPR_STRING_LITERAL:
                            fprintf(output_file, ".string %s\n", d->value->string_literal);
                            break;
                        case EXPR_CHAR_LITERAL:
                            fprintf(output_file, ".quad %d\n", d->value->string_literal[1]);
                            break;
                    }
                } else {
                    fprintf(output_file, "\t.quad 0\n");
                }
            } else {
                curr_local_count++;
                d->symbol->loc = curr_local_count + curr_param_count;
                if(d->value) {
                    expr_codegen(d->value);
                    fprintf(output_file, "MOVQ %s, %s\n",
                        scratch_name(d->value->reg),
                        symbol_codegen(d->symbol));
                    scratch_free(d->value->reg);


                }

            }
        }
    } else {
        curr_param_count = 0;
        return_count = 0;
        fprintf(output_file, ".text\n");
        fprintf(output_file, ".global %s\n", d->symbol->name);
        fprintf(output_file, "%s:\n", d->symbol->name);
        fprintf(output_file, "PUSHQ %%rbp\n");
        fprintf(output_file, "MOVQ %%rsp, %%rbp\n");
        codegen_set_params(d->type->params);
        int num_params = count_params(d->type->params);
        int num_locals = count_locals(d->code->body);
        // printf("num_locals: %d\n", num_locals);
        // printf("num_params: %d\n", num_params);
        fprintf(output_file, "SUBQ $%d, %%rsp\n", 8*(num_locals + num_params));
        fprintf(output_file, "PUSHQ %%rbx\n");
        fprintf(output_file, "PUSHQ %%rbp\n");
        fprintf(output_file, "PUSHQ %%rsp\n");
        fprintf(output_file, "PUSHQ %%r12\n");
        fprintf(output_file, "PUSHQ %%r13\n");
        fprintf(output_file, "PUSHQ %%r14\n");
        fprintf(output_file, "PUSHQ %%r15\n");

        stmt_codegen(d->code, d);
    }

    if(d->code && return_count == 0) {
        fprintf(output_file, "MOVQ $0, %%rax\n");
        fprintf(output_file, "JMP .%s_epilogue\n", d->name);
        fprintf(output_file, ".%s_epilogue:\n", d->name);
        fprintf(output_file, "POPQ %%r15\n");
        fprintf(output_file, "POPQ %%r14\n");
        fprintf(output_file, "POPQ %%r13\n");
        fprintf(output_file, "POPQ %%r12\n");
        fprintf(output_file, "POPQ %%rsp\n");
        fprintf(output_file, "POPQ %%rbp\n");
        fprintf(output_file, "POP %%rbx\n");
        codegen_pop_params(d->type->subtype->params);
        fprintf(output_file, "MOVQ %%rbp, %%rsp\n");
        fprintf(output_file, "POPQ %%rbp\n");
        fprintf(output_file, "RET\n");
    }

    decl_codegen(d->next);

}

int count_params(struct param_list * p) {
    int count = 0;
    while(p) {
        count++;
        p = p->next;
    }

    return count;
}


int count_locals(struct stmt *s) {
    if(!s) {
        return 0;
    }
    int count = 0;
    switch(s->kind) {
        case STMT_DECL:
            count++;
            break;
        case STMT_IF_ELSE:
            count += count_locals(s->body);
            count += count_locals(s->else_body);
            break;
        case STMT_BLOCK:
            count += count_locals(s->body);
        case STMT_RETURN:
            return 0;
            break;
        default:
            break;
    }

    return count + count_locals(s->next);
}


void codegen_set_params(struct param_list *params) {
    struct param_list * p = params;
    if(p) {
        fprintf(output_file, "PUSH %%rdi\n");
        curr_param_count++;
        p->symbol->loc = curr_param_count;
        p = p->next;
    }

    if(p) {
        fprintf(output_file, "PUSH %%rsi\n");
        curr_param_count++;
        p->symbol->loc = curr_param_count;
        p = p->next;
    }

    if(p) {
        fprintf(output_file, "PUSH %%rdx\n");
        curr_param_count++;
        p->symbol->loc = curr_param_count;
        p = p->next;
    }

    if(p) {
        fprintf(output_file, "PUSH %%rcx\n");
        curr_param_count++;
        p->symbol->loc = curr_param_count;
        p = p->next;
    }

    if(p) {
        fprintf(output_file, "PUSH %%r8\n");
        curr_param_count++;
        p->symbol->loc = curr_param_count;
        p = p->next;
    }

    if(p) {
        fprintf(output_file, "PUSH %%r9\n");
        curr_param_count++;
        p->symbol->loc = curr_param_count;
        p = p->next;
    }

    if(p) {
        printf("error: too many function arguments");
        exit(1);
    }

    return;
}


void codegen_pop_params(struct param_list *params) {
    struct param_list *p = params;
    if(p) {
        fprintf(output_file, "POP %%r9\n");
        p = p->next;
    }

    if(p) {
        fprintf(output_file, "POP %%r8\n");
        p = p->next;
    }

    if(p) {
        fprintf(output_file, "POP %%rcx\n");
        p = p->next;
    }

    if(p) {
        fprintf(output_file, "POP %%rdx\n");
        p = p->next;
    }

    if(p) {
        fprintf(output_file, "POP %%rsi\n");
        p = p->next;
    }

    if(p) {
        fprintf(output_file, "PUSH %%rdi\n");
        p = p->next;
    }
}




void stmt_codegen(struct stmt *s, struct decl * d) {
    // printf("in stmt_codegen\n");
    if(!s) {
        // printf("no body\n");
        return;
    }

    // printf("s->kind: %d\n", s->kind);
    switch(s->kind) {
        case STMT_EXPR:
            expr_codegen(s->expr);
            scratch_free(s->expr->reg);
            break;
        case STMT_DECL:
            decl_codegen(s->decl);
            break;
        case STMT_RETURN:
            expr_codegen(s->expr);
            fprintf(output_file, "MOVQ %s, %%rax\n", 
                scratch_name(s->expr->reg));
                fprintf(output_file, "JMP .%s_epilogue\n", d->name);
            scratch_free(s->expr->reg);
            // function epilogue (cleanup)
            
            if(return_count == 0) {
                fprintf(output_file, ".%s_epilogue:\n", d->name);
                fprintf(output_file, "POPQ %%r15\n");
                fprintf(output_file, "POPQ %%r14\n");
                fprintf(output_file, "POPQ %%r13\n");
                fprintf(output_file, "POPQ %%r12\n");
                fprintf(output_file, "POPQ %%rsp\n");
                fprintf(output_file, "POPQ %%rbp\n");
                fprintf(output_file, "POP %%rbx\n");
                codegen_pop_params(d->type->subtype->params);
                fprintf(output_file, "MOVQ %%rbp, %%rsp\n");
                fprintf(output_file, "POPQ %%rbp\n");
                fprintf(output_file, "RET\n");
            }
            return_count++;
            break;
        case STMT_BLOCK:
            stmt_codegen(s->body, d);
            break;
        case STMT_PRINT:
            if (!s->expr) {
                break;
            }

            struct expr *curr = s->expr->next;
            struct type *t;
            while (curr) {
                expr_codegen(curr);
                if (curr->symbol) {
                    if(curr->symbol->type->kind == TYPE_FUNCTION) {
                        t = curr->symbol->type->subtype;
                    } else {
                        t = curr->symbol->type;
                    }
                    
                } else {
                    switch (curr->kind) {
                        case EXPR_INTEGER_LITERAL:
                            t = type_create(TYPE_INTEGER, NULL, NULL, NULL);
                            break;
                        case EXPR_CHAR_LITERAL:
                            t = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
                            break;
                        case EXPR_STRING_LITERAL:
                            t = type_create(TYPE_STRING, NULL, NULL, NULL);
                            break;
                        case EXPR_BOOLEAN_LITERAL:
                            t = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
                            break;
                        default:
                            switch (curr->return_kind) {
                                case EXPR_INTEGER_LITERAL:
                                    t = type_create(TYPE_INTEGER, NULL, NULL, NULL);
                                    break;
                                case EXPR_CHAR_LITERAL:
                                    t = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
                                    break;
                                case EXPR_STRING_LITERAL:
                                    t = type_create(TYPE_STRING, NULL, NULL, NULL);
                                    break;
                                case EXPR_BOOLEAN_LITERAL:
                                    t = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
                                    break;
                            }

                    }
                }
                if(curr->symbol) {
                    if(curr->symbol->kind == SYMBOL_GLOBAL) {
                        if(curr->symbol->type->kind == TYPE_STRING) {
                            fprintf(output_file, "LEA %s, %%rdi\n", curr->symbol->name);
                        } else if(curr->symbol->type->kind ==  TYPE_FUNCTION) {
                            fprintf(output_file, "MOVQ %s, %%rdi\n", scratch_name(curr->reg));
                        } else {
                            fprintf(output_file, "MOVQ %s, %%rdi\n", scratch_name(curr->reg));
                        }
                    } else {
                        fprintf(output_file, "MOVQ %s, %%rdi\n", scratch_name(curr->reg));
                    }
                } else {
                    fprintf(output_file, "MOVQ %s, %%rdi\n", scratch_name(curr->reg));
                }
                scratch_free(curr->reg);

                fprintf(output_file, "PUSHQ %%r10\n");
                fprintf(output_file, "PUSHQ %%r11\n");


                switch (t->kind) {
                    case TYPE_INTEGER:
                        fprintf(output_file, "CALL print_integer\n");
                        break;
                    case TYPE_STRING:
                        fprintf(output_file, "CALL print_string\n");
                        break;
                    case TYPE_CHARACTER:
                        fprintf(output_file, "CALL print_character\n");
                        break;
                    case TYPE_BOOLEAN:
                        fprintf(output_file, "CALL print_boolean\n");
                        break;
                }

                fprintf(output_file, "POPQ %%r11\n");
                fprintf(output_file, "POPQ %%r10\n");
                scratch_free(curr->reg);

                curr = curr->next;
            }
            break;
        case STMT_FOR:
            expr_codegen(s->init_expr);
            int start_label = label_create();
            int end_label = label_create();
            fprintf(output_file, "%s:\n", label_name(start_label));
            expr_codegen(s->expr);
            if(!s->expr) {
                int dummy_reg = scratch_alloc();
                fprintf(output_file, "MOVQ $1, %s\n", scratch_name(dummy_reg));  // no middle expr
                fprintf(output_file, "CMP $0, %s\n", scratch_name(dummy_reg)); 
            } else {
                fprintf(output_file, "CMP $0, %s\n", scratch_name(s->expr->reg)); // middle expr
            }
            fprintf(output_file, "JE %s\n", label_name(end_label));
            stmt_codegen(s->body, d);
            expr_codegen(s->next_expr);
            fprintf(output_file, "JMP %s\n", label_name(start_label));
            fprintf(output_file, "%s:\n", label_name(end_label));
            if(s->init_expr) {
                scratch_free(s->init_expr->reg);
            }
            if(s->expr) {
                scratch_free(s->expr->reg);
            }
            if(s->next_expr) {
                scratch_free(s->next_expr->reg);
            }
            break;
        case STMT_IF_ELSE:;
            int else_label = label_create();
            int done_label = label_create();
            expr_codegen(s->expr);
            fprintf(output_file, "CMP $0, %s\n", scratch_name(s->expr->reg));
            scratch_free(s->expr->reg);
            if(s->else_body) {
                fprintf(output_file, "JE %s\n", label_name(else_label));
                stmt_codegen(s->body, d);
                fprintf(output_file, "JMP %s\n", label_name(done_label));
                fprintf(output_file, "%s:\n", label_name(else_label));
                stmt_codegen(s->else_body, d);
            } else {
                fprintf(output_file, "JE %s\n", label_name(done_label));
                stmt_codegen(s->body, d);
            }
            fprintf(output_file, "%s:\n", label_name(done_label));
            break;

            
    }

    stmt_codegen(s->next, d);
 }



const char * symbol_codegen(struct symbol *s) {
    char str[BUFSIZ];
    switch(s->kind) {
        case SYMBOL_GLOBAL:
            return s->name;
        case SYMBOL_PARAM:;
            sprintf(str, "%d(%%rbp)", -8*(s->loc));
            return strdup(str);
        case SYMBOL_LOCAL:;  
            sprintf(str, "%d(%%rbp)", -8*(s->loc));
            return strdup(str);
    }
}