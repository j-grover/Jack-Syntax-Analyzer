#include "JackTokeniser.h"
#include "myxml.h"
using namespace std;

// Outputs tokens and their content using myxml class
static void outputToken()
{
	string element = JackTokeniser::tokenName;
	string content = JackTokeniser::tokenString;

	myxml::addnode(element);
	myxml::addcontent(content);
	myxml::closenode(element);
}

int main()
{
	// Gets next token from JackTokeniser class
	jacktoken token = JackTokeniser::nextToken();

	// Calls for output of starting tokens tag
	myxml::addnode("tokens");

	// Outputs token until eof is reached
	while ( token != eof ) {
		outputToken();
		token = JackTokeniser::nextToken();
	}

	// Calls for output of closing tokens tag
	myxml::closenode("tokens");
	return 0;
}
