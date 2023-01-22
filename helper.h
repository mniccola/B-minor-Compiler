#ifndef HELPER_H
#define HELPER_H

#include "tokens.h"
#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
extern int yylex();
extern char *yytext;
extern int yyleng;

int print_scan();

void print_tabs(int indent);
void print_resolution(struct symbol * s);
void print_type_error(struct type *lt, struct type *rt, struct expr *e);
#endif