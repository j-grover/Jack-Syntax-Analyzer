#include <iostream>
#include "myxml.h"

using namespace std ;

// record current node level for indentation
static int indents = 0 ;
// record if the current node has content written
static bool hascontent = true ;

// output the open tag for the named element
void myxml::addnode(string element)
{
	// indent open tag if current node has no content
	if ( !hascontent )
	{
		cout << "\r\n" ;	// this would be endl but "\r\n" will match the nand2Tetris examples

		// output 2 spaces for each level of indent - to match nand2Tetris examples
		for ( int i = 0 ; i < indents ; i++ )
		{
			cout << "  " ;
		}
	}

	// increase the indent by 1
	indents++ ;

	// now output open tag for element - but no newline
	cout << "<" << element << ">" ;

	// clear hascontent
	hascontent = false ;
}

// it will translate <,>,&,'," to HTML entities
void myxml::addcontent(string content)
{
	// record the output of content
	hascontent = true ;

	// output each character one at a time - after translation
	cout << " " ;		// put a space before the content - this is incorrect but it will match the nand2Tetris examples
	for ( int i = 0 ; i < content.length() ; i++ )
	{
		char ch = content[i] ;
		switch(ch)
		{
		case '<':
			cout << "&lt;" ;
			break ;
		case '>':
			cout << "&gt;" ;
			break ;
		case '&':
			cout << "&amp;" ;
			break ;
		case '\'':
			cout << "&apos;" ;
			break ;
		case '"':
			cout << "&quot;" ;
			break ;
		default:
			cout << ch ;
			break ;
		}
	}
	cout << " " ;		// put a space after the content - this is incorrect but it will match the nand2Tetris examples
	
}

// output close tag for the named element
void myxml::closenode(string element)
{
	// decrease the indent by 1
	indents-- ;

	// put close tag on a newline if current node has no content
	if ( !hascontent )
	{
		cout << "\r\n" ;
		// output 2 spaces for each level of indent - to match nand2Tetris examples
		for ( int i = 0 ; i < indents ; i++ )
		{
			cout << "  " ;
		}
	}

	// clear hascontent
	hascontent = false ;

	// now output the close tag for element followed by a newline
	cout << "</" << element << ">" ;

	// if closing the root add a newline
	if ( indents == 0 )
	{
		cout << "\r\n" ;	// this would be endl but "\r\n" will match the nand2Tetris examples
	}
}
