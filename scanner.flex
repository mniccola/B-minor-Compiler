%{
#include "tokens.h"
%}

DIGIT [0-9]
LETTER [a-zA-z]

%%
(" "|\t|\n|\r)
array       {return TOKEN_ARRAY;}
auto        {return TOKEN_AUTO;}
boolean     {return TOKEN_BOOLEAN;}
char        {return TOKEN_CHAR;}
else        {return TOKEN_ELSE;}
false       {return TOKEN_FALSE;}
for         {return TOKEN_FOR;}
function    {return TOKEN_FUNCTION;}
if          {return TOKEN_IF;}
integer     {return TOKEN_INTEGER;}
print       {return TOKEN_PRINT;}
return      {return TOKEN_RETURN;}
string      {return TOKEN_STRING;}
true        {return TOKEN_TRUE;}
void        {return TOKEN_VOID;}
while       {return TOKEN_WHILE;}


[A-Za-z_][A-Za-z0-9_]{0,254}      {return TOKEN_IDENTIFIER;}
{DIGIT}+                    {return TOKEN_INTEGER_LITERAL;}
'(\\.|[^\\])'                     {return TOKEN_CHARACTER_LITERAL;}
\"(\\.|[^"\\\n]){0,255}\"         {return TOKEN_STRING_LITERAL;}
\/\*([^*]|(\*[^\/]))*\*?\*\/
\/\/(.*)\n

\;                            {return TOKEN_SEMICOLON;}
,                            {return TOKEN_COMMA;}
\(                           {return TOKEN_LEFT_PAREN;}
\)                           {return TOKEN_RIGHT_PAREN;}
\[                           {return TOKEN_LEFT_BRACKET;}
\]                           {return TOKEN_RIGHT_BRACKET;}
\{                           {return TOKEN_LEFT_CURLYBRACE;}
\}                           {return TOKEN_RIGHT_CURLYBRACE;}
!                            {return TOKEN_LOGICAL_NOT;}
\^                           {return TOKEN_EXPONENTIATION;}
\*                           {return TOKEN_MULTIPLICATION;}
\/                           {return TOKEN_DIVISION;}
%                            {return TOKEN_MOD;}
\+                           {return TOKEN_PLUS;}
\-                            {return TOKEN_MINUS;}   
\<                           {return TOKEN_LT;}
\<=                          {return TOKEN_LE;}
\>                           {return TOKEN_GT;}
\>=                          {return TOKEN_GE;}
==                           {return TOKEN_EQ;}
!=                           {return TOKEN_NE;}
&&                           {return TOKEN_LOGICAL_AND;}
\|\|                         {return TOKEN_LOGICAL_OR;}
=                            {return TOKEN_ASSIGNMENT;}
\+\+                           {return TOKEN_POST_ADD;}
\-\-                           {return TOKEN_POST_SUB;}
:                            {return TOKEN_COLON;}
\?                           {return TOKEN_QUESTION;}
<<EOF>>                      {return TOKEN_EOF;}
.                            {return TOKEN_ERROR;}

%%
int yywrap() {return 1;}
