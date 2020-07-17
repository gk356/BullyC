//*****************************************************************************
// purpose: driver for CSE 4713 / 6713 example recursive descent parser
//  author: Joe Crumpton
//*****************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

#include "lexer.h"
#include "productions.h"

extern "C"
{
// global variables used by flex
extern FILE *yyin;         // input stream
extern FILE *yyout;        // output stream
extern int   yylex();      // the generated lexical analyzer

}



// global variables used by this expression parser
int nextToken = 0;         // token returned from yylex
int level = 0;             // current indentation level

//*****************************************************************************
// The main processing loop
int main( int argc, char* argv[] )
{
  // Set the input stream
  if (argc > 1) 
  {
    cout << "INFO: Using the file " << argv[1] << " for input" << endl;
    yyin = fopen(argv[1], "r");
    if (!yyin)
    {
      cerr << "   ERROR: input file not found" << endl;
      return (1);
    }
  }
  else 
  {
    cout << "INFO: Using stdin for input, use EOF to end input " << endl;
    cout << "      Windows EOF is Ctrl+z, Linux EOF is Ctrl+d " << endl;
    yyin = stdin;
  }

  // Set the output stream
  yyout = stdout;
  
  // Get the first token
  nextToken = yylex();
  
  // Fire up the parser!
  try
  {
    if( !isFirstofProgram() ) // Check for IDENT | INT_LIT | (
      throw "first token does not start with '{' ";

    // Process <expr> production
    program(); 

    if(nextToken != TOK_EOF)
      throw "end of file expected, but there is more here!";
  }
  catch( char const *errmsg )    
  {
    cout << endl << "***ERROR: " << endl;
    cout << "On line number " << line_number << ": ";
    cout << errmsg << endl;
    return 1;
  }

  cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;
  
  //Prints out the Symbol Table
  cout << endl << "User Defined Symbols: " << endl;
  Symbol_Table::iterator it;
  for(it = symbolTable.begin(); it != symbolTable.end(); ++it){
    cout << it->first << endl;
  }  

  return 0;
}

