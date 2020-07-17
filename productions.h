/*******************************************************************************
  Name: Gautam KC             NetID: gk356
  Course: CSE 4713              Assignment: Program 3
  Programming Environment: Window Subsystem for Linux
  Purpose of File: Contains the production rules of the grammar
*******************************************************************************/

#ifndef PRODUCTIONS_H
#define PRODUCTIONS_H

extern int nextToken;
extern int level;
extern int line_number;  

extern "C"
{
    extern char* yytext;
    extern int yylex();
}

//Defining symbol tanle and the datatype for it
typedef map<string, float> Symbol_Table;

// Defining a symboltable
Symbol_Table symbolTable;

// Defining symbol table using vector
//std::vector<string> symbolTable;

// forward declaration of production functions
float expr();
float term();
float factor();

// My production declaration
void program();
void statement();
float unary();
float relation();
float boolean();
void assignment();
void get();
void output();
void con();
void w();

// forward declarations of first set checking functions
bool isFirstOfExpr();
bool isFirstOfTerm();
bool isFirstOfFactor();
bool isFirstofProgram();
bool isFirstofUnary();
bool isFirstofRelation();
bool isFirstofBoolean();
bool isFirstOfAssignment();
bool isFirstofGet();
bool isFirstofOutput();
bool isFirstofConditional();
bool isFirstofStatement();
bool isFirstofW();
// forward declaration of function used to help with tree nesting
string spaces();

//*****************************************************************************
// <program> --> { { statement } }
void program(){
	static int programCount = 0; // Count the number of <expr>'s
	int currentProgramCount = ++programCount;

	level = level + 1;
	cout << spaces() << "enter <program>" << currentProgramCount <<endl;
	cout << spaces() << "--> found: " << yytext << endl;

	// reading next token 
	nextToken = yylex();
	while(isFirstofStatement()){
		statement();
	}

	if(nextToken != TOK_CLOSEBRACE){
		throw "missing '}' at end of program" ;
	}
	cout<<spaces() << " exit <program> " <<currentProgramCount << endl;
	level = level -1;
	nextToken = yylex();

}

//******************************************************************************
// <statement> --> <assignment> |<get> |<output> |<conditional> |<while>
void statement(){
	
	static int statementCount = 0; // Count the number of <expr>'s
	int currentStatementCount = ++statementCount;

	level = level +1 ;
	cout<< spaces() <<"enter <statement> " << currentStatementCount <<endl;

	switch(nextToken){
		case TOK_LET:
			assignment();
			break;

		case TOK_READ:
			get();
			break;

		case TOK_PRINT:
			output();
			break;

		case TOK_IF:
			con();
			break;
		case TOK_WHILE:
			w();
			break;
		default:
			throw "incorrect keyword for statement";

	}
	cout << spaces() << "exit <statement> " << currentStatementCount <<endl;
	level = level - 1;
	
}

//******************************************************************************
// <assignment> --> let ID := <expression> ;
void assignment(){
	float value;
	//string strID;
	static int assignCount = 0; // Count the number of <expr>'s
	int currentAssignCount = ++assignCount;
	
	level = level + 1;
	cout << spaces() << "enter <assignment> " << currentAssignCount << endl;

	cout << spaces() << "--> found: " << yytext <<endl;
	nextToken = yylex();

	if(nextToken != TOK_IDENTIFIER){
		throw "missing identifier in assignment statement";		
	}
	cout << spaces() << "--> found ID: " << yytext << endl;
	string strID = yytext;

	nextToken = yylex();	
	
	if(nextToken != TOK_ASSIGN){
		throw "missing ':=' in assignment statement ";
	}
	cout << spaces() << "--> found: " << yytext << endl;
	nextToken = yylex();
	
	if(isFirstOfExpr()){
		value = expr();
	}
	else{
		throw "missing expression in assignment statement ";
	}
	// Check if the identifier is in the symbol table and add if it is not
	Symbol_Table::iterator it = symbolTable.find(strID);
	//cout << "symbol table: " << (it == symbolTable.end()) <<endl;

	if(it == symbolTable.end()){
		symbolTable.insert(pair<string,float>(strID, 1.0));
	}
	

	it = symbolTable.find(strID);
	it->second = value;

	if(nextToken == TOK_SEMICOLON){
		cout << spaces() << "--> found: " << yytext << endl;
		nextToken = yylex();
	}
	else{
		throw "missing ';' at the end of assignment statement ";
	}

	cout << spaces() << "exit <assignment>" << currentAssignCount <<endl;
	level = level - 1;
}

//*****************************************************************************
// <term> --> <factor> { ( * | / ) <factor> }
float term()
{
	float value = 0;
	float value2;
	static int termCount = 0; // Count the number of <term>'s
	int currentTermCount = ++termCount;

	char const* Ferr = "<factor> does not start with 'IDENT' | 'INT_LIT' | '('";

	level = level + 1;
	cout << spaces() << "enter <term> " << currentTermCount << endl;

	// We next expect to see a <factor>
	if (isFirstOfFactor())
		value = factor();
	else
		throw Ferr;

	// As long as the next token is * or /, keep parsing <factor>'s
	while (nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE)
	{
		cout << spaces() << "-->found: " << yytext << endl;
		int token = nextToken;
		nextToken = yylex();
		if (isFirstOfFactor())
			value2 = factor();
		else
			throw Ferr;
		if(token == TOK_MULTIPLY)
			value = value * value2;
		else if(token == TOK_DIVIDE)
			value = value / value2;
	}

	cout << spaces() << "exit <term>" << currentTermCount << endl;
	level = level - 1;
	return value;
}

//******************************************************************************
// <boolean>--> <relation> [( <| > | == ) <relation>]
float boolean(){
	float value = 0;
	float value2;
	static int boolCount = 0; // Count the number of <expr>'s
	int currentBoolCount = ++boolCount;

	char const* Berr =
		"<relation> does not start with 'IDENT' | 'INT_LIT' | '(' | 'not' | '-' ";

	level = level + 1;
	cout << spaces() << "enter <boolean> " << currentBoolCount << endl;

	// We next expect to see a <term>
	if (isFirstofRelation())
		value = relation();
	else
		throw Berr;

	// As long as the next token is + or -, keep parsing <term>'s
	if (nextToken == TOK_LESSTHAN || nextToken == TOK_GREATERTHAN || nextToken == TOK_EQUALTO)
	{
		cout << spaces() << "-->found: " << yytext << endl;
		int token = nextToken;
		nextToken = yylex();
		if (isFirstofRelation())
			value2 = relation();
		else
			throw Berr;
		
		switch(token){
			case TOK_LESSTHAN:
				value = value < value2;
				break;
			case TOK_GREATERTHAN:
				value = value > value2;
				break;
			default:
				value = value == value2;
		}
	}

	cout << spaces() << "exit <boolean> " << currentBoolCount << endl;
	level = level - 1;
	return value;

}

//*******************************************************************************
// <relaation> --> <term> {(+|-) <term> }
float relation(){

	float value = 0;
	float value2;
	static int relCount = 0; // Count the number of <expr>'s
	int currentRelCount = ++relCount;

	char const* Rerr =
		"<term> does not start with 'IDENT' | 'INT_LIT' | '(' | 'not' | '-' ";

	level = level + 1;
	cout << spaces() << "enter <relation> " << currentRelCount << endl;

	// We next expect to see a <term>
	if (isFirstOfTerm())
		value = term();
	else
		throw Rerr;

	// As long as the next token is + or -, keep parsing <term>'s
	while (nextToken == TOK_PLUS || nextToken == TOK_MINUS)
	{
		cout << spaces() << "-->found " << yytext << endl;
		int token = nextToken;
		nextToken = yylex();
		if (isFirstOfTerm())
			value2 = term();
		else
			throw Rerr;
		
		switch(token){
			case TOK_PLUS:
				value = value + value2;
				break;
			
			case TOK_MINUS:
				value = value - value2;
		}
	}

	cout << spaces() << "exit <relation> " << currentRelCount << endl;
	level = level - 1;
	return value;
}

//*******************************************************************************
// <uanry> --> ID | INTLIT | (expression)
float unary(){
	float value = 0;
	Symbol_Table::iterator it;
	static int unaryCount = 0;
	int currentUnaryCount = ++unaryCount;	
	//string strID;
	//char const* Terr = "statement does not start with recognized keyword";

	level = level + 1;
	cout << spaces() << "enter <unary> " << currentUnaryCount << endl;

	// we next expect to see a 
	switch(nextToken){
		case TOK_IDENTIFIER:
			cout << spaces() << "--> found ID: " << yytext << endl;
			it = symbolTable.find(yytext); // Checking identifier in symbol table
			
			if( it == symbolTable.end()){
				throw "unitialized identifier used in expression";
			}
			value = it->second;
			nextToken = yylex(); // Read past what we have found
			break;

		case TOK_INTLIT:
			cout << spaces() << "--> found INTLIT: " << yytext << endl;
			value = (int)atoi(yytext);
			nextToken = yylex();
			break;

		case TOK_OPENPAREN:
			// We expect ( <expr> )
			//      parse it
			cout << spaces() << "-->found: " << yytext << endl;
			nextToken = yylex();
			if (!isFirstOfExpr()) // Check for 'IDENT' | 'INT_LIT' | (
				throw "first token not 'IDENT' | 'INT_LIT' | '('";

			value = expr();

			if (nextToken == TOK_CLOSEPAREN) {
				cout << spaces() << "-->found:" << yytext << endl;
				nextToken = yylex();
			}
			else
				throw "<expression> does not end with )";
			break;

		default:
			// If we made it to here, syntax error
			throw "uninitialized identifier used in expression";
	}
	cout << spaces() << "exit <unary>" << currentUnaryCount <<endl;
	level = level -1 ;
	return value;
}

//*****************************************************************************
// <expr> --> <term> { ( and | or ) <term> }
float expr()
{
	float value = 0;
	float value2;
	static int exprCount = 0; // Count the number of <expr>'s
	int currentExprCount = ++exprCount;

	char const* Terr =
		"<boolean> does not start with 'IDENT' | 'INT_LIT' | '(' || 'not' | '-' ";

	level = level + 1;
	cout << spaces() << "enter <expr> " << currentExprCount << endl;

	// We next expect to see a <term>
	if (isFirstofBoolean())
		value = boolean();
	else
		throw Terr;

	// As long as the next token is + or -, keep parsing <term>'s
	while (nextToken == TOK_AND || nextToken == TOK_OR)
	{
		cout << spaces() << "-->found " << yytext << endl;
		int token = nextToken;
		nextToken = yylex();
		if (isFirstofBoolean())
			value2 = boolean();
		else
			throw Terr;
		switch(token){
			case TOK_OR:
				value = value || value2;
				break;
			case TOK_AND:
				value = value && value2;

		}
	}

	cout << spaces() << "exit <expr> " << currentExprCount << endl;
	level = level - 1;
	return value;
}

//*****************************************************************************
// <factor> --> [ not | - ] <unary>
float factor()
{
	float value = 0;
	static int factorCount = 0; // Count the number of <factor>'s
	int currentFactorCount = ++factorCount;
	
	//char const* Ferr ="<factor> does not start with 'not' | '-' | 'IDENT' | 'INT_LIT' | '('";

	level = level + 1;
	cout << spaces() << "enter <factor> " << currentFactorCount << endl;

	// Determine what token we have
	if(nextToken == TOK_NOT || nextToken == TOK_MINUS){
		cout<<spaces() << "--> found: "<< yytext <<endl;
		nextToken = yylex();
	}
	if(isFirstofUnary()){
		value = unary();
	}
	else{
		throw "factor doesnot start with 'not' | '-' | 'ID' | 'INTLIT' | '(' ";
	}

	cout << spaces() << "exit <factor> " << currentFactorCount << endl;
	level = level - 1;
	return value;
	
}

//*****************************************************************************
// <get> --> read [ STRINGLIT ] ID ;
void get(){
	static int getCount = 0; // Count the number of <expr>'s
	int currentGetCount = ++getCount;

	level = level + 1;
	cout << spaces() << "enter <get> " << currentGetCount << endl;

	cout << spaces() << "--> found: " <<yytext <<endl;
	nextToken = yylex();
	
	if(nextToken == TOK_STRINGLIT){
		cout << spaces() << "--> found: " << yytext << endl;
		nextToken = yylex();
	}
	if(nextToken == TOK_IDENTIFIER){
		cout << spaces() << "--> found: " << yytext << endl;
		string strID = yytext;
		Symbol_Table::iterator it = symbolTable.find(strID);
		if(it == symbolTable.end()){
			symbolTable.insert(pair<string,float> (strID, 0.0));
		}
	}
	else{
		throw "missing identifier in read statement";
	}
	nextToken = yylex();
	if(nextToken == TOK_SEMICOLON){
		cout << spaces() << "--> found: " << yytext << endl;
		nextToken = yylex();
	}
	else{
		throw "missing ';' at the end of read statement ";
	}
	
	cout << spaces() << "exit <GET>" << currentGetCount <<endl;
	level = level - 1;
}

//*****************************************************************************
// <output> --> print [ STRINGLIT ] [ ID ];
void output(){
	static int outputCount = 0; // Count the number of <expr>'s
	int currentOutputCount = ++outputCount;

	level = level + 1;
	cout << spaces() << "enter <output> " << currentOutputCount <<endl;

	cout << spaces() << "-->found: " << yytext << endl;
	nextToken = yylex();

	if(nextToken == TOK_STRINGLIT){
		cout << spaces() << "--> found: " << yytext << endl;
		nextToken = yylex();
	}

	if(nextToken == TOK_IDENTIFIER){
		cout << spaces() << "--> found ID: " <<yytext <<endl;
		string strID = yytext;
		Symbol_Table::iterator it = symbolTable.find(strID);
		if(it == symbolTable.end()){
			throw "uninitialized identifer in read statement";
		}
		nextToken = yylex();
	}

	if(nextToken != TOK_SEMICOLON){
		throw "missing ';' at the end of print statement";
	}
	else{
		cout << spaces() << "--> found: " << yytext << endl;
	}
	cout << spaces() << "exit <output> " << currentOutputCount <<endl;
	level = level - 1;
	nextToken = yylex();
}
//*****************************************************************************
// <conditional> --> if (<expression>) <program> [else <program>]
void con(){
	static int conCount = 0; // Count the number of <expr>'s
	int currentConCount = ++conCount;

	level = level + 1;
	cout << spaces() << "enter <conditional>" << currentConCount <<endl;

	cout << spaces() << "--> found: " << yytext <<endl ;
	nextToken = yylex();

	if(nextToken == TOK_OPENPAREN){
		cout << spaces() << "--> found: " << yytext << endl;
		nextToken = yylex();
	}
	else{
		throw "of boolean does not start with '(' ";
	}
	if(isFirstOfExpr()){
		expr();
	}
	else{
		throw "missing expression in if statement";
	}
	if(nextToken == TOK_CLOSEPAREN){
		cout << spaces() << "--> found: " <<yytext <<endl;
		nextToken = yylex();
	}
	else{
		throw "if boolean does not end with ')'";
	}
	if(isFirstofProgram()){
		program();
	}
	if(nextToken == TOK_ELSE){
		cout << spaces() << "--> found: " << yytext << endl;
		nextToken = yylex();
	}
	if(isFirstofProgram()){
		program();
	}
	cout << spaces() << "exit <conditional> " << currentConCount << endl;
	level = level - 1;
}

//*****************************************************************************
// <while> --> while ( <expression> ) <program>
void w(){
	static int wCount = 0; // Count the number of <expr>'s
	int currentWCount = ++wCount;

	level = level + 1;
	cout << spaces() << "enter <while>" << currentWCount << endl;


	cout << spaces() << "--> found: " << yytext << endl;
	
	nextToken = yylex();
		
	if(nextToken == TOK_OPENPAREN){
		cout << spaces() << "--> found: " << yytext <<endl;
		nextToken = yylex();
	}
	else{
		throw "while boolean does not start with '(' ";
	}
	if(isFirstOfExpr()){
		expr();
	}		
	else{
		throw "missing expression in while statement ";
	}
	if(nextToken = TOK_CLOSEPAREN){
		cout << spaces() << "--> found: " << yytext <<endl;
		nextToken = yylex();
	}
	else{
		throw "while boolean does not end with ')'";
	}
	if(isFirstofProgram()){
		program();
	}
	else{
		throw "while does not include a program ";
	}

}
//*****************************************************************************
bool isFirstofUnary(){
	return nextToken == TOK_OPENPAREN ||
		nextToken == TOK_IDENTIFIER ||
		nextToken == TOK_INTLIT;
}

//*****************************************************************************
bool isFirstOfExpr() {
	return nextToken == TOK_IDENTIFIER ||
		nextToken == TOK_INTLIT ||
		nextToken == TOK_OPENPAREN || 
		nextToken == TOK_NOT ||
		nextToken == TOK_MINUS;
}
//*****************************************************************************
bool isFirstOfTerm() {
	return nextToken == TOK_IDENTIFIER ||
		nextToken == TOK_INTLIT ||
		nextToken == TOK_OPENPAREN ||
		nextToken == TOK_NOT ||
		nextToken == TOK_MINUS;
}
//*****************************************************************************
bool isFirstOfFactor() {
	return nextToken == TOK_IDENTIFIER ||
		nextToken == TOK_INTLIT ||
		nextToken == TOK_OPENPAREN ||
		nextToken == TOK_MINUS ||
		nextToken == TOK_NOT;
}

//*****************************************************************************
bool isFirstofRelation(){
	return nextToken == TOK_NOT ||
		nextToken == TOK_MINUS ||
		nextToken == TOK_OPENPAREN ||
		nextToken == TOK_IDENTIFIER ||
		nextToken == TOK_INTLIT;
}

//*****************************************************************************
bool isFirstofBoolean(){
	return nextToken == TOK_NOT ||
		nextToken == TOK_MINUS ||
		nextToken == TOK_OPENPAREN ||
		nextToken == TOK_IDENTIFIER ||
		nextToken == TOK_INTLIT; 
}

//*****************************************************************************
bool isFirstOfAssignment(){
	return nextToken == TOK_LET;
}

//*****************************************************************************
bool isFirstofGet(){
	return nextToken == TOK_READ;
}
//*****************************************************************************
bool isFirstofProgram(){
	return nextToken == TOK_OPENBRACE;
}

//*****************************************************************************
bool isFirstofStatement(){
	return nextToken == TOK_LET ||
		nextToken == TOK_READ ||
		nextToken == TOK_PRINT ||
		nextToken == TOK_IF ||
		nextToken == TOK_WHILE;
}

//*****************************************************************************
bool isFirstofOutput(){
	return nextToken == TOK_PRINT;
}

//*****************************************************************************
bool isFirstofCon(){
	return nextToken == TOK_IF;
}
//*****************************************************************************
bool isFirstofW(){
	return nextToken == TOK_WHILE;
}
//*****************************************************************************
string spaces() {
	string str(level * 3, ' ');
	return str;
}

#endif