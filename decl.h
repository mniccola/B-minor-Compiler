
#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include "scope.h"
#include "param_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
};

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next );
void decl_print( struct decl *d, int indent );
int decl_resolve(struct decl *d);
void decl_typecheck(struct decl *d);

#endif


