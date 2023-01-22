#include "helper.h"

int print_scan() {
    while(1) {
            enum yytokentype t = yylex();
            switch(t) {
                case 0:
                    return 0;
                    break;
                case TOKEN_ARRAY:
                    printf("TOKEN_ARRAY\n");
                    break;
                case TOKEN_AUTO:
                    printf("TOKEN_AUTO\n");
                    break;
                case TOKEN_BOOLEAN:
                    printf("TOKEN_BOOLEAN\n");
                    break;
                case TOKEN_CHAR:
                    printf("TOKEN_CHAR\n");
                    break;
                case TOKEN_FALSE:
                    printf("TOKEN_FALSE\n");
                    break;
                case TOKEN_FOR:
                    printf("TOKEN_FOR\n");
                    break;
                case TOKEN_FUNCTION:
                    printf("TOKEN_FUNCTION\n");
                    break;
                case TOKEN_IF:
                    printf("TOKEN_IF\n");
                    break;
                case TOKEN_INTEGER:
                    printf("TOKEN_INTEGER\n");
                    break;
                case TOKEN_PRINT:
                    printf("TOKEN_PRINT\n");
                    break;
                case TOKEN_RETURN:
                    printf("TOKEN_RETURN\n");
                    break;
                case TOKEN_STRING:
                    printf("TOKEN_STRING\n");
                    break;
                case TOKEN_TRUE:
                    printf("TOKEN_TRUE\n");
                    break;
                case TOKEN_VOID:
                    printf("TOKEN_VOID\n");
                    break;
                case TOKEN_WHILE:
                    printf("TOKEN_WHILE\n");
                    break;
                case TOKEN_STRING_LITERAL:;

                    char str_literal[BUFSIZ];
                    int count = 0;
                    for (int i = 1; i < yyleng - 1; i++) {
                        if(yytext[i] == '\\') {
                            if(yytext[i + 1] == 'n') {
                                str_literal[count] = '\n';
                                count++;
                                i++;
                                continue;
                            } else {
                                i++;
                                str_literal[count] = yytext[i];
                                count++;
                                continue;
                            }
                            continue;
                        } else {
                            str_literal[count] = yytext[i];
                            count++;
                        }
                    }

                    str_literal[yyleng] = '\0';
                    printf("STRING LITERAL %s\n", str_literal);
                    break;
                case TOKEN_CHARACTER_LITERAL:;
                    char c;
                    for (int i = 1; i < yyleng - 1; i++) {
                        if(yytext[i] == '\\') {
                            if(yytext[i+1] == 'n') {
                                c = '\n';
                                i++;
                                continue;
                            } else if (yytext[i+1] == '0') {
                                c = '\0';
                                i++;
                                continue;
                            } else {
                                c = yytext[i+1];
                                i++;
                                continue;
                            }
                            continue;
                        } else {
                            c = yytext[i];
                            continue;
                        }
                    }
        

                    printf("CHARACTER LITERAL %c\n", c);
                    break;
                case TOKEN_INTEGER_LITERAL:
                    printf("INTEGER LITERAL %s\n", yytext);
                    break;
                case TOKEN_IDENTIFIER:
                    printf("IDENTIFIER  %s\n", yytext);
                    break;
                case TOKEN_SEMICOLON:
                    printf("SEMICOLON\n");
                    break;
                case TOKEN_COMMA:
                    printf("COMMA\n");
                    break;
                case TOKEN_LEFT_PAREN:
                    printf("LEFT_PAREN\n");
                    break;
                case TOKEN_RIGHT_PAREN:
                    printf("RIGHT_PAREN\n");
                    break;
                case TOKEN_LEFT_BRACKET:
                    printf("LEFT_BRACKET\n");
                    break;
                case TOKEN_RIGHT_BRACKET:
                    printf("RIGHT_BRACKET\n");
                    break;
                case TOKEN_LEFT_CURLYBRACE:
                    printf("LEFT_CURLYBRACE\n");
                    break;
                case TOKEN_RIGHT_CURLYBRACE:
                    printf("RIGHT_CURLYBRACE\n");
                    break;
                case TOKEN_LOGICAL_NOT:
                    printf("LOCICAL_NOT\n");
                    break;
                case TOKEN_EXPONENTIATION:
                    printf("EXPONENTIATION\n");
                    break;
                case TOKEN_MULTIPLICATION:
                    printf("MULTIPLICATION\n");
                    break;
                case TOKEN_DIVISION:
                    printf("DIVISION\n");
                    break;
                case TOKEN_MOD:
                    printf("MOD\n");
                    break;
                case TOKEN_LT:
                    printf("LT\n");
                    break;
                case TOKEN_LE:
                    printf("LE\n");
                    break;
                case TOKEN_GT:
                    printf("GT\n");
                    break;
                case TOKEN_GE:
                    printf("GE\n");
                    break;
                case TOKEN_EQ:
                    printf("EQ\n");
                    break;
                case TOKEN_NE:
                    printf("NE\n");
                    break;
                case TOKEN_LOGICAL_AND:
                    printf("LOGIAL_AND\n");
                    break;
                case TOKEN_LOGICAL_OR:
                    printf("LOCIAL_OR\n");
                    break;
                case TOKEN_ASSIGNMENT:
                    printf("ASSIGNMENT\n");
                    break;
                case TOKEN_PLUS:
                    printf("PLUS\n");
                    break;
                case TOKEN_MINUS:
                    printf("MINUS\n");
                    break;
                case TOKEN_COLON:
                    printf("COLON\n");
                    break;
                case TOKEN_QUESTION:
                    printf("QUESTION\n");
                    break;
                default:
                    break;
        }
    }
}



void print_tabs(int indent) {
    // printf("printing %d tabs ", indent);
    for(int i = 0; i < indent; i++) {
        printf("\t");
    }
    // printf(" $$ ");
}

void print_resolution(struct symbol * s) {
    switch (s->kind) {
        case SYMBOL_GLOBAL:
            printf("%s resolves to global %s\n", s->name, s->name);
            break;
        case SYMBOL_LOCAL:
            printf("%s resolves to local %d\n", s->name, s->which);
            break;
        case SYMBOL_PARAM:
             printf("%s resolves to param %d\n", s->name, s->which);
    }
}

void print_type_error(struct type *lt, struct type *rt, struct expr *e) {
    if(lt) {
        type_print(lt);
        printf(" (");
        expr_print(e->left);
        printf(") ");
    }
    if(rt) {
        printf("and ");
        type_print(rt);
        printf(" (");
        expr_print(e->right);
        printf(")");
    }
        
    printf("\n");
}
