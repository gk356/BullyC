/*******************************************************************************
  Name: Gautam KC             NetID: gk356
  Course: CSE 4713              Assignment: Program 3
  Programming Environment: Window Sybsystem for Linux
  Purpose of File: Contains the rule for the lexer analyzer
*******************************************************************************/
%option noyywrap
%{
#include "lexer.h"


// global variable to hold current line number being read
int line_number = 1;

%}

%%

 /* Keywords */ 
if                     { return TOK_IF; }
else                   { return TOK_ELSE; }
for                    { return TOK_FOR; }
while                  { return TOK_WHILE; }
print                  { return TOK_PRINT; }
return                 { return TOK_RETURN; }
continue               { return TOK_CONTINUE; }
break                  { return TOK_BREAK; }
read                   { return TOK_READ; } 
let                    { return TOK_LET; }          



 /* Datatype Specifiers */
int                    { return TOK_INT; }
float                  { return TOK_FLOAT; }
string                 { return TOK_STRING; }


 /* Punctuation */
\;                      { return TOK_SEMICOLON; }
\(                     { return TOK_OPENPAREN; }
\)                      { return TOK_CLOSEPAREN; }
\{                      { return TOK_OPENBRACE; }
\}                      { return TOK_CLOSEBRACE; }

 /* Operators */
\+                      { return TOK_PLUS; }
\-                      { return TOK_MINUS; }
\*                      { return TOK_MULTIPLY; }
\/                      { return TOK_DIVIDE; }
\:=                     { return TOK_ASSIGN; }
\==                     { return TOK_EQUALTO; }
\<                      { return TOK_LESSTHAN; }
\>                      { return TOK_GREATERTHAN; }
\<>                     { return TOK_NOTEQUALTO; }
and                     { return TOK_AND; }
or                      { return TOK_OR; }
not                    { return TOK_NOT; }
length                  { return TOK_LENGTH;}



 /* Abstractions */

[0-9]+                  { return TOK_INTLIT; }
[0-9]+"."[0-9]*       { return TOK_FLOATLIT; }
\"[^\"]*\"              { return TOK_STRINGLIT; }
[A-Za-z][0-9a-zA-Z_]*   { return TOK_IDENTIFIER; }
\"[^\"]+                { return TOK_EOF_SL; }


 /* Eat any whitespace */

[ \t\r]+         /*nop*/

 /* Line counter incrementer */
[\n]                    { line_number++ ;}


 /* Found an unknown character */

.         { return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }


