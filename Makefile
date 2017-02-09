# default action is to compiler the tokeniser and parser
all: JackTokeniser JackParser

# add the names of other .h files used by JackTokeniser to TOKENH
TOKENH=myxml.h JackTokeniser.h
# add the names of other .cpp files used by JackTokeniser to TOKENCPP
TOKENCPP=jackmain.cpp myxml.cpp JackTokeniser.cpp

# recompile the tokeniser if any of its .h or .cpp files have changed
JackTokeniser: ${TOKENCPP} ${TOKENH}
	g++ -o JackTokeniser ${TOKENCPP}


# add the names of other .h files used by JackParser to PARSERH
PARSERH=myxml.h CompilationEngine.h JackTokeniser.h
# add the names of other .cpp files used by JackParser to PARSERCPP
PARSERCPP=parsermain.cpp myxml.cpp CompilationEngine.cpp JackTokeniser.cpp

# recompile the parser if any of its .h or .cpp files have changed
JackParser: ${PARSERCPP} ${PARSERH}
	g++ -o JackParser ${PARSERCPP}

# delete the executables
clean:
	rm -f JackTokeniser JackParser
