#include "tokens.h"
#include "helper.h"
#include "decl.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "param_list.h"
#include "scratch.h"
#include "codegen.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

extern FILE *yyin;
extern int yylex();
extern char *yytext;
extern int yyleng;
struct decl *ast;

int typecheck_errors = 0;

FILE * output_file;

int main(int argc, char *argv[]) {


    bool flag = false;
    char scan_file[BUFSIZ];
    if(argc > 4) {
        exit(1); // put a usage here
    }

    yyin = fopen(argv[2], "r");
    if(!yyin) {
        printf("could not open %s!\n", argv[2]);
        exit(1);
    }

    if(!strcmp(argv[1], "-scan")) {
        return print_scan();
    } else if(!strcmp(argv[1], "-parse")) {
        if(yyparse()==0) {
            printf("successful parse\n");
            return 0;
        } else {
            printf("parse failed!\n");
            return 1;
        }
    } else if (!strcmp(argv[1], "-print")) {
        
        if(yyparse() == 0) {
            if(!ast) {
                printf("Parse error: parser did not find a dec\n");
                exit(1);
            }   
            decl_print(ast, 0);
            exit(0);
        } else {
            exit(1);
        }
    } else if(!strcmp(argv[1], "-resolve")) {
        if(yyparse() == 0) {
            // printf("before scope enter\n");
            scope_enter();
            // printf("after scope enter\n");
            int status = decl_resolve(ast);
            if (status > 0) {
                exit(1);
            } else {
                exit(0);
            }
        } else {
            exit(1);
        }
    } else if(!strcmp(argv[1], "-typecheck")) { 
        if(yyparse() == 0) {
            scope_enter();
            int status = decl_resolve(ast);
            if(status > 0) {
                exit(1);
            }
            decl_typecheck(ast);
            if(typecheck_errors > 0) {
                exit(1);
            } else {
                exit(0);
            }
        }
    } else if(!strcmp(argv[1], "-codegen")) {
        // printf("in codegen main\n");
        char * input_file = argv[2];
        // printf("after file assignment");
        // first parse, resolve and typecheck.
        if(yyparse() == 0) {
            scope_enter();
            int status = decl_resolve(ast);
            if(status > 0) {
                exit(1);
            }
            decl_typecheck(ast);
            if(typecheck_errors > 0) {
                exit(1);
            }
        }

        output_file = fopen(argv[3], "w");
        fprintf(output_file, ".file \"%s\"\n", input_file);
        // printf("initing regs\n");
        scratch_init();
        decl_codegen(ast);
     }

    // printf("bottom of main\n");

    return 0;

    
}
