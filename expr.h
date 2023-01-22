#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"
#include "helper.h"
#include <stdlib.h>
#include <stdbool.h>
#include "scope.h"

typedef enum {
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_EXPONENT,
	EXPR_GT,
	EXPR_GE,
	EXPR_LT,
	EXPR_LE,
	EXPR_EQ,
	EXPR_NE,
	EXPR_POST_ADD,
	EXPR_POST_DEC,
	EXPR_NEG,
	EXPR_AND,
	EXPR_OR,
	EXPR_ASSIGN,
	EXPR_NOT,
	EXPR_FUNC_CALL,
	EXPR_ARR_INDX,
	EXPR_TERNARY,
	EXPR_ARR_LITERAL,
	EXPR_NAME,
	EXPR_PRINT_LIST,
	EXPR_INTEGER_LITERAL,
	EXPR_BOOLEAN_LITERAL,
	EXPR_CHAR_LITERAL,
	EXPR_STRING_LITERAL,
	/* many more kinds of exprs to add here */
} expr_t;


struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	const char *name;
	int literal_value;
	const char * string_literal;
	struct symbol *symbol;
	struct expr *next;
	struct expr *indx_next;
	struct expr * arr_literal_next;
	struct expr * func_params;
	expr_t return_kind;
	int reg;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right);

struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_char_literal( char *c );
struct expr * expr_create_string_literal( const char *str );
struct expr * expr_create_func_call(const char *str, struct expr *param_next);
struct expr * expr_create_arr_index(const char *, struct expr *indx_list);
struct expr * expr_create_ternary(struct expr *left, struct expr *right, struct expr *next);
struct expr * expr_create_array_literal(struct expr *arr_literal_next);

void expr_print( struct expr *e);

bool expr_check_precedence(struct expr *upper, struct expr *lower);
bool is_inList(expr_t val, expr_t arr [], int length);
int get_precedence(struct expr *e);

int expr_resolve(struct expr *e);
struct type *expr_typecheck(struct expr *e);

#endif
