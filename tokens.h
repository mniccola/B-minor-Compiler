/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_TOKENS_H_INCLUDED
# define YY_YY_TOKENS_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_EOF = 258,
    TOKEN_ARRAY = 259,
    TOKEN_AUTO = 260,
    TOKEN_BOOLEAN = 261,
    TOKEN_CHAR = 262,
    TOKEN_ELSE = 263,
    TOKEN_FALSE = 264,
    TOKEN_FOR = 265,
    TOKEN_FUNCTION = 266,
    TOKEN_IF = 267,
    TOKEN_INTEGER = 268,
    TOKEN_PRINT = 269,
    TOKEN_RETURN = 270,
    TOKEN_STRING = 271,
    TOKEN_TRUE = 272,
    TOKEN_VOID = 273,
    TOKEN_WHILE = 274,
    TOKEN_INTEGER_LITERAL = 275,
    TOKEN_CHARACTER_LITERAL = 276,
    TOKEN_IDENTIFIER = 277,
    TOKEN_STRING_LITERAL = 278,
    TOKEN_SEMICOLON = 279,
    TOKEN_FUNC_CALL = 280,
    TOKEN_LEFT_PAREN = 281,
    TOKEN_RIGHT_PAREN = 282,
    TOKEN_LEFT_BRACKET = 283,
    TOKEN_RIGHT_BRACKET = 284,
    TOKEN_LEFT_CURLYBRACE = 285,
    TOKEN_RIGHT_CURLYBRACE = 286,
    TOKEN_LOGICAL_NOT = 287,
    TOKEN_EXPONENTIATION = 288,
    TOKEN_MULTIPLICATION = 289,
    TOKEN_DIVISION = 290,
    TOKEN_MOD = 291,
    TOKEN_LT = 292,
    TOKEN_LE = 293,
    TOKEN_GT = 294,
    TOKEN_GE = 295,
    TOKEN_EQ = 296,
    TOKEN_NE = 297,
    TOKEN_LOGICAL_AND = 298,
    TOKEN_LOGICAL_OR = 299,
    TOKEN_ASSIGNMENT = 300,
    TOKEN_PLUS = 301,
    TOKEN_MINUS = 302,
    TOKEN_COLON = 303,
    TOKEN_QUESTION = 304,
    TOKEN_COMMA = 305,
    TOKEN_POST_ADD = 306,
    TOKEN_POST_SUB = 307,
    TOKEN_ERROR = 308
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 54 "parser.bison" /* yacc.c:1909  */

	struct decl *decl;
	struct stmt *stmt;
	struct expr *expr;
	struct type *type;
	struct symbol * symbol;
	struct param_list *param_list;
	char * ident;

#line 118 "tokens.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TOKENS_H_INCLUDED  */
