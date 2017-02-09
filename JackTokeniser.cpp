#include "JackTokeniser.h"
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <limits>
#include <iostream>
#include <map>
using namespace std;

// Map for jack predfined keywords
static map<string, jacktoken> keywords;
// Map for jack symbols
static map<string, jacktoken> symbols;

// Initialises the keywords and symbols map
static void createTokens()
{
	keywords["class"]	= keyword;
	keywords["method"]	= keyword;
	keywords["function"]	= keyword;
	keywords["constructor"]	= keyword;
	keywords["int"]		= keyword;
	keywords["boolean"]	= keyword;
	keywords["char"]	= keyword;
	keywords["void"]	= keyword;
	keywords["var"]		= keyword;
	keywords["static"]	= keyword;
	keywords["field"]	= keyword;
	keywords["let"]		= keyword;
	keywords["do"]		= keyword;
	keywords["if"]		= keyword;
	keywords["else"]	= keyword;
	keywords["while"]	= keyword;
	keywords["return"]	= keyword;
	keywords["true"]	= keyword;
	keywords["false"]	= keyword;
	keywords["null"]	= keyword;
	keywords["this"]	= keyword;

	symbols["{"]	= symbol;
	symbols["}"]	= symbol;
	symbols["("]	= symbol;
	symbols[")"]	= symbol;
	symbols["["]	= symbol;
	symbols["]"]	= symbol;
	symbols["."]	= symbol;
	symbols[","]	= symbol;
	symbols[";"]	= symbol;
	symbols["+"]	= symbol;
	symbols["-"]	= symbol;
	symbols["*"]	= symbol;
	symbols["/"]	= symbol;
	symbols["&"]	= symbol;
	symbols["|"]	= symbol;
	symbols["<"]	= symbol;
	symbols[">"]	= symbol;
	symbols["="]	= symbol;
	symbols["~"]	= symbol;
}

// Static variables for tokens
string JackTokeniser::tokenName;
jacktoken JackTokeniser::token;
string JackTokeniser::tokenString;
int JackTokeniser::lineNo;

jacktoken JackTokeniser::nextToken()
{
	static bool initialise = false;
	// Initialise maps on first call to nextToken
	if (!initialise) {
		initialise = true;
		createTokens();
	}

	char currChar;	// Current char from cin
	string currToken;	// Current token being built
	JackTokeniser jack;	// Class object
	string currType;	// Current token type of token
	bool strFlag = false;	// Used to indicate string token
	bool status = true;	// Used to indicate valid token

	// Loop until current token is valid and EOF is not reached
	while (status && currChar != EOF) {

		// Get current and next char in input
		currChar = cin.get();
		char nextChar = cin.peek();

		// Count line number
		if (currChar == '\n' || currChar == EOF) {
			lineNo++;
		}

		stringstream ss;
		string next;
		ss << nextChar;
		ss >> next;
		// Sets token type of next char in input
		string nextType = jack.tokenType(next);

		// Ignore single line commments
		if (currChar == '/' && nextChar == '/') {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			lineNo++;
		}

		// Ignore multi-line comments
		else if (currChar == '/' && nextChar == '*') {
			cin.ignore(numeric_limits<streamsize>::max(), '/');
		}

		// Ignoring whitespace
		else if (!isspace(currChar)) {

			// Build current token and get token type
			currToken += currChar;
			currType = jack.tokenType(currToken);

			// String found in input
			if (currChar == '\"') {
				strFlag = true;
			}

			// Keep spaces part of strings
			if (strFlag && nextChar == ' ') {
				currToken += nextChar;
			}

			// Valid token found
			if (currType != "ERROR") {

				// Set token properties to keyword type
				if (currType == "keyword") {
					tokenString = currToken;
					token = keyword;
					currToken.clear();
					status = false;
				}
				// Set token properties to symbol type
				else if (currType == "symbol") {
					tokenString = currToken;
					token = symbol;
					currToken.clear();
					status = false;
				}
				// Set token properties to stringConstant type
				else if (currType == "stringConstant") {
					// Remove " " from string constant
					tokenString = currToken.substr(1, currToken.length() - 2);
					token = stringConstant;
					currToken.clear();
					strFlag = false;
					status = false;
				}
				// Set token properties to integerConstant type and look for multi digit integers
				else if (currType == "integerConstant" && (nextChar == ' ' || nextType != "integerConstant")) {
					tokenString = currToken;
					token = integerConstant;
					currToken.clear();
					status = false;
				}
				// Set token properties to identifier type and make sure identifier isn't a string
				else if (currType == "identifier" && ((nextChar == ' ' || nextType == "symbol"
						|| nextChar == '\n' || nextChar == '\r')) && !strFlag) {
					tokenString = currToken;
					token = identifier;
					currToken.clear();
					status = false;
				}
			}
		}
	}
	tokenName = currType;

	// Return eof when end of input
	if (currChar == EOF) {
		return eof;
	}
	return token;
}

bool JackTokeniser::checkKeyword(string token)
{
	// Search for token in keywords map
	map<string, jacktoken>::iterator it;
	it = keywords.find(token);
	return (it != keywords.end());
}

bool JackTokeniser::checkSymbol(string token)
{
	// Search for token in symbols map
	map<string, jacktoken>::iterator it;
	it = symbols.find(token);
	return (it != symbols.end());
}

bool JackTokeniser::checkIdentifier(string token)
{
	// If first char is not a digit
	if (!isdigit(token[0])) {
		bool status = true;
		for (int i = 0; i < token.size(); i++) {
			// Check for letters, digits and underscore
			if (!isalpha(token[i]) && !isdigit(token[i]) && token[i] != '_') {
				bool status = false;
				break;
			}
		}
		return status;
	}
	return false;
}

bool JackTokeniser::checkIntConst(string token)
{
	stringstream ss(token);
	int integer;
	// Check token is a number
	if (!(ss >> integer).fail()) {
		int num = atoi(token.c_str());
		// Check for valid range of 0 to 32767
		return (num >= 0 && num <= 32767);
	}
	return false;
}

bool JackTokeniser::checkStringConst(string token)
{
	int length = token.length();
	// Check if token starts and ends with quotes
	if (token[0] == '\"' && token[length - 1] == '\"' && length != 1) {
		bool status = true;
		for (int i = 0; i < token.size(); i++) {
			// Check token does not include double quote or newline
			if (token[i] == '\"' || token[i] == '\n') {
				bool status = false;
				break;
			}
		}
		return status;
	}
	return false;
}

string JackTokeniser::tokenType(string token)
{
	// Token is a keyword
	if (checkKeyword(token))
		return "keyword";

	// Token is a symbol
	if (checkSymbol(token))
		return "symbol";

	// Token is a integer constant
	if (checkIntConst(token))
		return "integerConstant";

	// Token is a string constant
	if (checkStringConst(token))
		return "stringConstant";

	// Token is a identifier
	if (checkIdentifier(token))
		return "identifier";

	// Not a valid token
	return "ERROR";
}
