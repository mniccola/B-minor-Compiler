
 %token TOKEN_EOF
 %token TOKEN_ARRAY
 %token TOKEN_AUTO
 %token TOKEN_BOOLEAN
 %token TOKEN_CHAR
 %token TOKEN_ELSE
 %token TOKEN_FALSE
 %token TOKEN_FOR
 %token TOKEN_FUNCTION
 %token TOKEN_IF
 %token TOKEN_INTEGER
 %token TOKEN_PRINT
 %token TOKEN_RETURN
 %token TOKEN_STRING
 %token TOKEN_TRUE
 %token TOKEN_VOID
 %token TOKEN_WHILE
 %token TOKEN_INTEGER_LITERAL
 %token TOKEN_CHARACTER_LITERAL
 %token TOKEN_IDENTIFIER
 %token TOKEN_STRING_LITERAL
 %token TOKEN_SEMICOLON
 %token TOKEN_FUNC_CALL
 %token TOKEN_LEFT_PAREN
 %token TOKEN_RIGHT_PAREN
 %token TOKEN_LEFT_BRACKET
 %token TOKEN_RIGHT_BRACKET
 %token TOKEN_LEFT_CURLYBRACE
 %token TOKEN_RIGHT_CURLYBRACE
 %token TOKEN_LOGICAL_NOT
 %token TOKEN_EXPONENTIATION
 %token TOKEN_MULTIPLICATION
 %token TOKEN_DIVISION
 %token TOKEN_MOD
 %token TOKEN_LT
 %token TOKEN_LE
 %token TOKEN_GT
 %token TOKEN_GE
 %token TOKEN_EQ
 %token TOKEN_NE
 %token TOKEN_LOGICAL_AND
 %token TOKEN_LOGICAL_OR
 %token TOKEN_ASSIGNMENT
 %token TOKEN_PLUS
 %token TOKEN_MINUS
 %token TOKEN_COLON
 %token TOKEN_QUESTION
 %token TOKEN_COMMA
 %token TOKEN_POST_ADD
 %token TOKEN_POST_SUB
 %token TOKEN_ERROR

 %union {
	struct decl *decl;
	struct stmt *stmt;
	struct expr *expr;
	struct type *type;
	struct symbol * symbol;
	struct param_list *param_list;
	char * ident;
};

%type <decl> program decl_list decl 
%type <stmt> stmt secondary_stmt either_if matched_if code_block expr_stmt stmt_list stmt_epsilon
%type <expr> expr expr_epsilon expr_fickle expr_list ternary term term1 term2 term3 term4 term5 term_arrList termA termB factor assignment_terms func_call index_list index factor_list factor_item index_or_name factor_list_item
%type <param_list> def_list def_seq def_item
%type <type> atomicType funcType
%type <ident> ident

%{

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "expr.h"
#include "decl.h"
#include "stmt.h"
#include "param_list.h"

struct decl *ast;

extern char *yytext;
extern int yylex();
extern int yyerror(char *str );
%}

%%

program : decl_list
		{ ast = $1; return 0;}
		| TOKEN_ERROR
		{ printf("scan error: invalid character read\n"); return 1;}
		;

decl	: ident TOKEN_COLON atomicType TOKEN_SEMICOLON
		{ $$ = decl_create($1, $3, NULL, NULL, NULL);}
		| ident TOKEN_COLON atomicType TOKEN_ASSIGNMENT assignment_terms TOKEN_SEMICOLON
		{ $$ = decl_create($1, $3, $5, NULL, NULL);}
		| ident TOKEN_COLON TOKEN_FUNCTION funcType TOKEN_LEFT_PAREN def_list TOKEN_RIGHT_PAREN TOKEN_SEMICOLON
		{ $$ = decl_create($1, type_create(TYPE_FUNCTION, $4, $6, NULL), NULL, NULL, NULL);}
		| ident TOKEN_COLON TOKEN_FUNCTION funcType TOKEN_LEFT_PAREN def_list TOKEN_RIGHT_PAREN TOKEN_ASSIGNMENT code_block
		{ $$ = decl_create($1, type_create(TYPE_FUNCTION, $4, $6, NULL), NULL, $9, NULL);}
		;

assignment_terms : expr
				 { $$ = $1;}
				 | term_arrList
				 { $$ = $1;}
				 ;

decl_list: decl decl_list
		 { $1->next = $2; $$ = $1;}
		 |
		 { $$ = NULL;}
		 ;


func_call : ident TOKEN_LEFT_PAREN expr_epsilon TOKEN_RIGHT_PAREN
		  {$$ = expr_create_func_call($1, $3);}
		  ;

atomicType : TOKEN_INTEGER
		   { $$ = type_create(TYPE_INTEGER, NULL, NULL, NULL);}
		   | TOKEN_STRING
		   { $$ = type_create(TYPE_STRING, NULL, NULL, NULL);}
		   | TOKEN_CHAR
		   { $$ = type_create(TYPE_CHARACTER, NULL, NULL, NULL);}
		   | TOKEN_BOOLEAN
		   { $$ = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);}
		   | TOKEN_AUTO
		   { $$ = type_create(TYPE_AUTO, NULL, NULL, NULL);}
		   | TOKEN_ARRAY TOKEN_LEFT_BRACKET expr TOKEN_RIGHT_BRACKET atomicType
		   { $$ = type_create(TYPE_ARRAY, $5, NULL, $3);}
		   | TOKEN_ARRAY TOKEN_LEFT_BRACKET TOKEN_RIGHT_BRACKET atomicType
		   { $$ = type_create(TYPE_ARRAY, $4, NULL, NULL);}
		   |
		   { $$ = NULL;}
		   ;

funcType : TOKEN_INTEGER
		 { $$ = type_create(TYPE_INTEGER, NULL, NULL, NULL);}
		 | TOKEN_BOOLEAN
		 { $$ = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);}
		 | TOKEN_STRING
		 { $$ = type_create(TYPE_STRING, NULL, NULL, NULL);}
		 | TOKEN_CHAR
		 { $$ = type_create(TYPE_CHARACTER, NULL, NULL, NULL);}
		 | TOKEN_VOID
		 { $$ = type_create(TYPE_VOID, NULL, NULL, NULL);}
		 ;


def_list : def_seq
		 {$$ = $1;}
		|
		{$$ = NULL;}
		;

def_seq : def_item TOKEN_COMMA def_seq
		{$1->next = $3; $$ = $1;}
		| def_item
		{ $$ = $1;}
		;

def_item : ident TOKEN_COLON atomicType
		 { $$ = param_list_create($1, $3, NULL);}
		 ;



stmt	: secondary_stmt
		{ $$ = $1;}
		| either_if
		{ $$ = $1;}
		| TOKEN_FOR TOKEN_LEFT_PAREN expr_fickle TOKEN_SEMICOLON expr_fickle TOKEN_SEMICOLON expr_fickle TOKEN_RIGHT_PAREN stmt
		{ $$ = stmt_create(STMT_FOR, NULL, $3, $5, $7, $9, NULL, NULL);}
		| decl
		{ $$ = stmt_create(STMT_DECL, $1, NULL, NULL, NULL, NULL, NULL, NULL);}
		;

secondary_stmt : TOKEN_PRINT expr_epsilon TOKEN_SEMICOLON
			   { struct expr *e = expr_create(EXPR_PRINT_LIST, NULL, NULL); e->next = $2; $$ = stmt_create(STMT_PRINT, NULL, NULL, e, NULL, NULL, NULL, NULL );}
			   | TOKEN_RETURN expr TOKEN_SEMICOLON
			   { $$ = stmt_create(STMT_RETURN, NULL, NULL, $2, NULL, NULL, NULL, NULL);}
			   | code_block
			   { $$ = $1;}
			   ;

either_if : TOKEN_IF TOKEN_LEFT_PAREN expr TOKEN_RIGHT_PAREN stmt
		  { $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3, NULL, $5, NULL, NULL);}
	   	  | TOKEN_IF TOKEN_LEFT_PAREN expr TOKEN_RIGHT_PAREN matched_if TOKEN_ELSE stmt
		  { $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3, NULL, $5, $7, NULL);}
	      ;

matched_if : TOKEN_IF TOKEN_LEFT_PAREN expr TOKEN_RIGHT_PAREN matched_if TOKEN_ELSE matched_if
		   { $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3, NULL, $5, $7, NULL);}
		   | TOKEN_FOR TOKEN_LEFT_PAREN expr_fickle TOKEN_SEMICOLON expr_fickle TOKEN_SEMICOLON expr_fickle TOKEN_RIGHT_PAREN matched_if
		   { $$ = stmt_create(STMT_FOR, NULL, $3, $5, $7, $9, NULL, NULL);}
		   | secondary_stmt
		   { $$ = $1;}
		   ;


code_block : TOKEN_LEFT_CURLYBRACE stmt_epsilon TOKEN_RIGHT_CURLYBRACE
		   {$$ = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, $2, NULL, NULL);}
		   ;

expr_fickle : expr
			{$$ = $1;}
			|
			{ $$ = NULL;}
			;

expr_stmt : expr TOKEN_SEMICOLON
		  {$$ = stmt_create(STMT_EXPR, NULL, NULL, $1, NULL, NULL, NULL, NULL);}
		  ;

stmt_list : stmt stmt_list
		  {$1->next = $2; $$ = $1;}
		  | stmt
		  {$$ = $1;}
		  | expr_stmt stmt_list
		  {$1->next = $2; $$ = $1;}
		  | expr_stmt
		  {$$ = $1;}
		  ;

stmt_epsilon : stmt_list
			 { $$ = $1;}
			 | 
			 { $$ = NULL;}
			 ;



expr_list : expr TOKEN_COMMA expr_list
		  { $1->next = $3; $$ = $1;}
		  | expr
		  { $$ = $1;}

expr_epsilon : expr_list
			 {$$ = $1;}
			 | 
			 { $$ = NULL;}
			 ;


expr	: expr TOKEN_ASSIGNMENT termA
		{ $$ = expr_create(EXPR_ASSIGN, $1, $3);}
		| ternary 
		{ $$ = $1; }
		| termA
		{ $$ = $1;}

ternary : expr TOKEN_ASSIGNMENT termA TOKEN_QUESTION termA TOKEN_COLON termA
		{ $$ = expr_create(EXPR_ASSIGN, $1, expr_create_ternary($3, $5, $7));}




termA	: termA TOKEN_LOGICAL_OR termB
		{ $$ = expr_create(EXPR_OR,  $1, $3);}
		| termB
		{ $$ = $1;}
		;

termB	: termB TOKEN_LOGICAL_AND term
		{ $$ = expr_create(EXPR_AND, $1, $3);}
		| term
		{ $$ = $1;}
		;


term	: term TOKEN_EQ term1
		{ $$ = expr_create(EXPR_EQ, $1, $3);}
		| term TOKEN_LT term1
		{ $$ = expr_create(EXPR_LT, $1, $3);}
		| term TOKEN_LE term1
		{ $$ = expr_create(EXPR_LE, $1, $3);}
		| term TOKEN_GT term1
		{ $$ = expr_create(EXPR_GT, $1, $3);}
		| term TOKEN_GE term1
		{ $$ = expr_create(EXPR_GE, $1, $3);}
		| term TOKEN_NE term1
		{ $$ = expr_create(EXPR_NE, $1, $3);}
		| term1
		{ $$ = $1;}
		;

term1	: term1 TOKEN_PLUS term2
		{ $$ = expr_create(EXPR_ADD, $1, $3);}
		| term1 TOKEN_MINUS term2
		{ $$ = expr_create(EXPR_SUB, $1, $3); }
		| term2
		{ $$ = $1;}
		;

term2	: term2 TOKEN_MULTIPLICATION term3
		{ $$ = expr_create(EXPR_MUL, $1, $3);}
		| term2 TOKEN_DIVISION term3
		{ $$ = expr_create(EXPR_DIV, $1, $3);}
		| term2 TOKEN_MOD term3
		{ $$ = expr_create(EXPR_MOD, $1, $3);}
		| term3
		{ $$ = $1;}
		;

term3	: term3 TOKEN_EXPONENTIATION term4
		{ $$ = expr_create(EXPR_EXPONENT, $1, $3);}
		| term4
		{ $$ = $1;}
		;

term4	: term4 TOKEN_POST_ADD 
		{ $$ = expr_create(EXPR_POST_ADD, $1, NULL);}
		| term4 TOKEN_POST_SUB
		{ $$ = expr_create(EXPR_POST_DEC, $1, NULL);}
		| term5
		{ $$ = $1;}
		| TOKEN_MINUS term5
		{ $$ = expr_create(EXPR_NEG, NULL, $2);}
		| TOKEN_LOGICAL_NOT term5
		{ $$ = expr_create(EXPR_NOT, NULL, $2);} 

term_arrList : TOKEN_LEFT_CURLYBRACE factor_list TOKEN_RIGHT_CURLYBRACE
			 { $$ = expr_create_array_literal($2);}
			 ;

term5 	: TOKEN_LEFT_PAREN expr TOKEN_RIGHT_PAREN
		{$$ = $2;}
		| factor
		{ $$ = $1;}
		;


factor	: TOKEN_INTEGER_LITERAL
		{ $$ = expr_create_integer_literal(atoi(yytext));}
		| TOKEN_STRING_LITERAL
		{ char * str = strdup(yytext); $$ = expr_create_string_literal(str);}
		| TOKEN_CHARACTER_LITERAL
		{ char * str = strdup(yytext); $$ = expr_create_char_literal(str);}
		| index_or_name
		| TOKEN_TRUE
		{ $$ = expr_create_boolean_literal(1);}
		| TOKEN_FALSE
		{ $$ = expr_create_boolean_literal(0);}
		| func_call
		{ $$ = $1;}
		;

index_or_name : ident index_list
			 {$$ = expr_create_arr_index($1, $2);}
		      | ident
			  { $$ = expr_create_name($1);}


index_list : index index_list
		   { $1->indx_next = $2; $$=$1;}
		   | index
		   { $$ = $1;}
		   ;

index : TOKEN_LEFT_BRACKET expr TOKEN_RIGHT_BRACKET
	  {$$ = $2;}
	  ;




factor_list	: factor_list_item factor_item
			{$1->next = $2; $$ = $1;}
			;

factor_item	: TOKEN_COMMA factor_list
			{$$ = $2;}
			| 
			{$$ = NULL;}
			;

factor_list_item : TOKEN_INTEGER_LITERAL
		{ $$ = expr_create_integer_literal(atoi(yytext));}
		| TOKEN_STRING_LITERAL
		{ char * str = strdup(yytext); $$ = expr_create_string_literal(str);}
		| TOKEN_CHARACTER_LITERAL
		{ char * str = strdup(yytext); $$ = expr_create_char_literal(str);}
		| index_or_name
		{ $$ = $1;}
		| TOKEN_TRUE
		{ $$ = expr_create_boolean_literal(1);}
		| TOKEN_FALSE
		{ $$ = expr_create_boolean_literal(0);}
		| func_call
		{ $$ = $1;}
		| TOKEN_LEFT_CURLYBRACE factor_list TOKEN_RIGHT_CURLYBRACE
		{ $$ = expr_create_array_literal($2);}
		;


ident : TOKEN_IDENTIFIER
	  {char * str = strdup(yytext); $$ = str;}
	  ;


%%



int yyerror(char *str) {
    printf("parse error: %s\n", str);
    return 1;
}
