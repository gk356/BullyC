/*******************************************************************************
  Name: Gautam KC             NetID: gk356
  Course: CSE 4713              Assignment: Program 3
  Programming Environment: Window Subsystem for Linux 
  Purpose of File: Contains the defination of token for lexer
*******************************************************************************/

#ifndef LEXER_H
#define LEXER_H

// List of token codes

#define TOK_EOF          5000  // end of file
#define TOK_EOF_SL       5001  // end of file while parsing string literal
#define TOK_UNKNOWN      6000  // unknown lexeme
#define TOK_IDENTIFIER   4000  // identiitifier token
#define TOK_INTLIT       4001  // integer literal
#define TOK_FLOATLIT     4002  // floating point literal
#define TOK_STRINGLIT    4003  // String literal

// List of token Keyword
#define TOK_IF           1001  // if token
#define TOK_ELSE         1002  // else 
#define TOK_FOR          1003  // for
#define TOK_WHILE        1004  // while
#define TOK_PRINT        1005  // print
#define TOK_RETURN       1006  // return
#define TOK_CONTINUE     1007  // continue
#define TOK_BREAK        1008  // break
#define TOK_READ         1010  // read
#define TOK_LET          1011  // llet

// List of token datatypes
#define TOK_INT          1100  // integer
#define TOK_FLOAT        1101  // float
#define TOK_STRING       1102  // string

// List of punctuaion token
#define TOK_SEMICOLON    2000  // semicolon ;
#define TOK_OPENPAREN    2001  // Open parenthesis (
#define TOK_CLOSEPAREN   2002  // Close parenthesis )
#define TOK_OPENBRACE    2003  // Open Braces {
#define TOK_CLOSEBRACE   2004  // Close Braces }

// List of operators token
#define TOK_PLUS         3000  // plus
#define TOK_MINUS        3001  // minus
#define TOK_MULTIPLY     3002  // multiply
#define TOK_DIVIDE       3003  // divide
#define TOK_ASSIGN       3004  // assignment
#define TOK_EQUALTO      3005  // equals-to
#define TOK_LESSTHAN     3006  // less than
#define TOK_GREATERTHAN  3007  // greater than
#define TOK_NOTEQUALTO   3008  // no equal
#define TOK_AND          3009  // and
#define TOK_OR           3010  // or
#define TOK_NOT          3011  // not
#define TOK_LENGTH       3012  // length


#endif

