#ifndef MYXML_H
#define MYXML_H

#include <string>

// the myxml class writes XML nodes and data to cout
// in element names must only include, letters, digits, '-' '_' and '.'
// it will translate <,>,&,'," to HTML entities
class myxml
{
public:
	static void addnode(std::string element) ;     // outputs <elementname>
	static void addcontent(std::string content) ;  // outputs the comment but translates &,<,>,',"
	static void closenode(std::string element) ;   // outputs </elementname>
};
#endif //MYXML_H
