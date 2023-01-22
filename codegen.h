#ifndef CODEGEN_H
#define CODEGEN_H

#include "expr.h"
#include "decl.h"
#include "stmt.h"
#include "scratch.h"
#include "label.h"
#include "symbol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void decl_codegen(struct decl *d);
const char * symbol_codegen(struct symbol *s);
void expr_codegen(struct expr *e);
void stmt_codegen(struct stmt *s, struct decl * d);

void codegen_set_params(struct param_list * p);
void codegen_pop_params(struct param_list *params);

int count_params(struct param_list * p);
int count_locals(struct stmt *s);

#endif