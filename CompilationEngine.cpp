#include "CompilationEngine.h"
#include <iostream>
#include <cstdlib>
using namespace std;

bool CompilationEngine::have(string expected)
{
	// Expected token is found
	if (expected == tokenContent)
		return true;
	return false;
}

void CompilationEngine::mustbe(string expected)
{
	JackTokeniser tokeniser;

	// Correct token or token type found
	if (expected != tokenContent && expected != tokenType)
	{
		// If token is a keyword
		if (tokeniser.checkKeyword(expected)) {
			cerr << "Syntax error on line: " << ++(JackTokeniser::lineNo)
				<< ", expected: " << expected << ", found " << tokenType
				<< "!" << endl;
			exit(-1);
		}
		else {
			cerr << "Syntax error on line: " << ++(JackTokeniser::lineNo)
				<< ", expected: " << expected << ", found " << tokenContent
				<< "!" << endl;
			exit(-1);
		}
	}
	else {
		outputToken();
	}
}

void CompilationEngine::getToken()
{
	// Update token variables used by outputToken()
	(void) JackTokeniser::nextToken();
	tokenType = JackTokeniser::tokenName;
	tokenContent = JackTokeniser::tokenString;
}

void CompilationEngine::outputToken()
{
	// Print token xml and update token
	myxml::addnode(tokenType);
	myxml::addcontent(tokenContent);
	myxml::closenode(tokenType);
	getToken();
}

void CompilationEngine::compileClass()
{
	// 'class' className '{' classVarDec* subroutineDec* '}'
	
	myxml::addnode("class");
	mustbe("class");
	mustbe("identifier");
	mustbe("{");

	while (tokenContent == "field" || tokenContent == "static")
		compileClassVarDec();

	while (tokenContent == "constructor" || tokenContent == "function"
			|| tokenContent == "method")
		compileSubroutineDec();

	mustbe("}");
	myxml::closenode("class");
}

void CompilationEngine::compileClassVarDec()
{
	// ('static' | 'field') type varName (',' varName)* ';'
	
	myxml::addnode("classVarDec");

	if (tokenContent == "static" || tokenContent == "field") {
		outputToken();

		if (tokenContent == "int" || tokenContent == "char"
				|| tokenContent == "boolean" || tokenType == "identifier") {
			outputToken();
			mustbe("identifier");

			while (tokenType == "symbol" && tokenContent == ",") {
				mustbe(",");
				outputToken();
			}
			mustbe(";");
		}
	}
	myxml::closenode("classVarDec");
}

void CompilationEngine::compileSubroutineDec()
{
	// ('constructor' | 'function' | 'method') ('void' | type) subroutineName
	// '(' parameterList ')' subroutineBody
	
	myxml::addnode("subroutineDec");

	if (tokenContent == "constructor" || tokenContent == "function"
			|| tokenContent == "method") {
		outputToken();

		if (tokenContent == "void" || (tokenContent == "int"
				|| tokenContent == "char" || tokenContent == "boolean"
				|| tokenType == "identifier")) {
			outputToken();
			mustbe("identifier");

			mustbe("(");
			compileParameterList();
			mustbe(")");
			compileSubroutineBody();
		}
	}
	myxml::closenode("subroutineDec");
}

void CompilationEngine::compileParameterList()
{
	// ((type varName) (',' type varName)*)?
	
	myxml::addnode("parameterList");

	if (tokenContent == "int" || tokenContent == "char"
			|| tokenContent == "boolean" || tokenType == "identifier") {
		outputToken();
		mustbe("identifier");

		while (tokenType == "symbol" && tokenContent == ",") {
			mustbe(",");
			outputToken();
			mustbe("identifier");
		}
	}
	myxml::closenode("parameterList");
}

void CompilationEngine::compileSubroutineBody()
{
	// '{' varDec* statements '}'
	
	myxml::addnode("subroutineBody");
	mustbe("{");

	while (tokenType == "keyword" && tokenContent == "var")
		compileVarDec();

	compileStatements();
	mustbe("}");
	myxml::closenode("subroutineBody");
}

void CompilationEngine::compileVarDec()
{
	// 'var' type varName (',' varName)* ';'
	
	myxml::addnode("varDec");
	mustbe("var");

	if (tokenContent == "int" || tokenContent == "char"
			|| tokenContent == "boolean" || tokenType == "identifier") {
		outputToken();
		mustbe("identifier");

		while (tokenType == "symbol" && tokenContent == ",") {
			mustbe(",");
			mustbe("identifier");
		}
		mustbe(";");
	}
	myxml::closenode("varDec");
}

bool CompilationEngine::isStatement(string token)
{
	// Used by compileStatements() to check for statements
	return (token == "let" || token == "if" || token == "while"
		|| token == "do" || token == "return");
}

void CompilationEngine::compileStatements()
{
	// statement*
	
	myxml::addnode("statements");

	while (tokenType == "keyword" && isStatement(tokenContent)) {
		if (have("let"))
			compileLet();
		else if (have("if"))
			compileIf();
		else if (have("while"))
			compileWhile();
		else if (have("do"))
			compileDo();
		else if (have("return"))
			compileReturn();
		else
			return;
	}
	myxml::closenode("statements");
}

void CompilationEngine::compileLet()
{
	// 'let' varName ('[' expression ']')? '=' expression ';'
	
	myxml::addnode("letStatement");
	mustbe("let");
	mustbe("identifier");

	if (have("[")) {
		mustbe("[");
		compileExpression();
		mustbe("]");
	}

	mustbe("=");
	compileExpression();
	mustbe(";");

	myxml::closenode("letStatement");
}

void CompilationEngine::compileIf()
{
	// 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
	
	myxml::addnode("ifStatement");
	mustbe("if");

	mustbe("(");
	compileExpression();
	mustbe(")");

	mustbe("{");
	compileStatements();
	mustbe("}");

	if (have("else")) {
		mustbe("else");
		mustbe("{");
		compileStatements();
		mustbe("}");
	}
	myxml::closenode("ifStatement");
}

void CompilationEngine::compileWhile()
{
	// 'while' '(' expression ')' '{' statements '}'
	
	myxml::addnode("whileStatement");
	mustbe("while");

	mustbe("(");
	compileExpression();
	mustbe(")");

	mustbe("{");
	compileStatements();
	mustbe("}");

	myxml::closenode("whileStatement");
}

void CompilationEngine::compileDo()
{
	// 'do' subroutineCall ';'
	
	myxml::addnode("doStatement");
	mustbe("do");

	compileSubroutineCall();

	mustbe(";");
	myxml::closenode("doStatement");
}

void CompilationEngine::compileReturn()
{
	// 'return' expression? ';'
	myxml::addnode("returnStatement");
	mustbe("return");

	if (have(";")) {
		mustbe(";");
	}
	else {
		compileExpression();
		mustbe(";");
	}
	myxml::closenode("returnStatement");
}

void CompilationEngine::compileExpression()
{
	// term (op term)*
	
	myxml::addnode("expression");
	compileTerm();

	while (isOperator(tokenContent)) {
		outputToken();
		compileTerm();
	}
	myxml::closenode("expression");
}

void CompilationEngine::compileTerm()
{
	// integerConstant | stringConstant | keywordConstant | varName
	// | varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term
	
	myxml::addnode("term");

	if (tokenType == "integerConstant") {
		outputToken();
	}
	else if (tokenType == "stringConstant") {
		outputToken();
	}
	else if (tokenType == "keyword") {
		outputToken();
	}
	else if (tokenContent == "(") {
		mustbe("(");
		compileExpression();
		mustbe(")");
	}
	else if (tokenContent == "-" || tokenContent == "~") {
		outputToken();
		compileTerm();
	}
	else if (tokenType == "identifier") {
		mustbe("identifier");

		if (tokenContent == "[") {
			mustbe("[");
			compileExpression();
			mustbe("]");
		}
		else if (tokenContent == "(") {
			mustbe("(");
			compileExpressionList();
			mustbe(")");
		}
		else if (tokenContent == ".") {
			mustbe(".");
			mustbe("identifier");

			mustbe("(");
			compileExpressionList();
			mustbe(")");
		}
	}
	myxml::closenode("term");
}

void CompilationEngine::compileSubroutineCall()
{
	// subroutineName '(' expressionList ')' | (className | varName)
	// '.' subroutineName '(' expressionList ')'
	
	mustbe("identifier");

	if (tokenContent == "(") {
		mustbe("(");
		compileExpressionList();
		mustbe(")");
	}
	else {
		mustbe(".");
		mustbe("identifier");

		mustbe("(");
		compileExpressionList();
		mustbe(")");
	}
}

void CompilationEngine::compileExpressionList()
{
	// (expression (',' expression)*)?
	
	myxml::addnode("expressionList");

	if (tokenContent != ")")
		compileExpression();

	while (tokenType == "symbol" && tokenContent == ",") {
		mustbe(",");
		compileExpression();
	}
	myxml::closenode("expressionList");
}

bool CompilationEngine::isOperator(string op)
{
	// Used by compileExpression() to check for operators
	return (op == "+" || op == "-" || op == "*" || op == "/" || op == "&"
		|| op == "|" || op == "<" || op == ">" || op == "=");
}
