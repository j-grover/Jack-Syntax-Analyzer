#ifndef COMPILATION_ENGINE_H
#define COMPILATION_ENGINE_H

#include "JackTokeniser.h"
#include "myxml.h"
#include <string>

// Gets input from JackTokeniser and emits a structured printout of the code,
// wrapped in XML tags
class CompilationEngine
{
public:
	// Returns true if current token is expected token, otherwise false
	bool have(std::string expected);

	// Checks current token with expected token, exits if different
	void mustbe(std::string expected);

	// Sets next token using JackTokeniser class
	void getToken();

	// Outputs token in XML tags
	void outputToken();

	// Compiles a complete class
	void compileClass();

	// Compiles a static declaration or a field declaration
	void compileClassVarDec();

	// Compiles a constructor, function or a method
	void compileSubroutineDec();

	// Compiles parameter list, not including the enclosing "()"
	void compileParameterList();

	// Compiles variable declarations and statements
	void compileSubroutineBody();

	// Compiles a var declaration
	void compileVarDec();

	// Returns true if token is a statement
	bool isStatement(std::string token);

	// Compiles a sequence of statements, not including the enclosing "{}"
	void compileStatements();

	// Compiles a let statement
	void compileLet();

	// Compiles an if statement, possibly with an else clause
	void compileIf();

	// Compiles a while statement
	void compileWhile();

	// Compiles a do statement
	void compileDo();

	// Compiles a return statement
	void compileReturn();

	// Compiles an expression
	void compileExpression();

	// Compiles a term
	void compileTerm();

	// Compiles a complete subroutine
	void compileSubroutineCall();

	// Compiles a comma-separated list of expressions
	void compileExpressionList();

	// Returns true if token is an operator, otherwise false
	bool isOperator(std::string op);

private:
	std::string tokenType;	// Stores current token type

	std::string tokenContent;	// Stores current token content
};

#endif // COMPILATION_ENGINE_H
