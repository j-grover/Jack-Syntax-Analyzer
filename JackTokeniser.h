#ifndef JACK_TOKENISER_H
#define JACK_TOKENISER_H

#include <string>

// List of tokens returned by Tokeniser, eof is returned when there is no more input
enum jacktoken { keyword, symbol, integerConstant, stringConstant, identifier, eof };

// Removes all comments and whitespace from the input stream and breaks it
// into Jack language tokens, as specified by the Jack grammar
class JackTokeniser
{
public:
	// Reads the next token from standard input
	static jacktoken nextToken();

	static std::string tokenName;	// Stoers the current token's name
	static jacktoken token;	// Stores the current token as an enum
	static std::string tokenString;	// Stores token as a string
	static int lineNo;	// Stores current line number

	// Returns true if token is a keyword
	bool checkKeyword(std::string token);
	
private:
	// Returns the type of the current token
	std::string tokenType(std::string token);

	// Returns true if token is a symbol
	bool checkSymbol(std::string token);

	// Returns true if token is a identifier
	bool checkIdentifier(std::string token);

	// Returns true if token is a integer constant
	bool checkIntConst(std::string token);

	// Returns true if token is a string constant
	bool checkStringConst(std::string token);
};

#endif // JACK_TOKENISER_H
