# Jack Syntax Analyzer
A syntax analyzer that parses Jack programs according to the Jack grammar, producing an XML file that renders the program's structure using marked-up text. Part of the nand2tetris project.

## Description
Tokeniser module:

The textual input is broken into a stream of tokens and classified into different lexical categories. Each token is printed on a new line along with its classification: symbol, keyword, identifier, integer constant or string constant. For each .jack file, a .xml output file can be generated.

Parser module:

The parser goes over the tokenized input and renders its grammatical structure using the Jack grammar. The parser scans the tokens for syntax errors and if found, writes to the output the line no. and the expected and found token.

Components:

- jackmain: Calls JackTokeniser methods to produce and output token
- JackTokeniser: Breaks input to tokens, removes whitespace and comments from input stream
- myxml: writes XML nodes and data to standard output
- CompilationEngine: Takes input from JackTokeniser and produces structured XML tokens
- parsermain: Calls CompilationEngine methods to compile code

## Running the program
```
$ make all
$ ./JackTokeniser < Input.jack > Output.xml
$ ./JackParser < Input.jack > Output.xml
```
Input.jack refers to the program file to be analyzed. Output.xml is optional, will contain the tokens.

## Test Example
Main.jack
```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/09/Square/Main.jack

/**
 * The Main class initializes a new Square Dance game and starts it.
 */
class Main {

    /** Initializes a new game and starts it. */
    function void main() {
        var SquareGame game;

        let game = SquareGame.new();
        do game.run();
		do game.dispose();

        return;
    }
}
```
Main.xml
```
<class>
  <keyword> class </keyword>
  <identifier> Main </identifier>
  <symbol> { </symbol>
  <subroutineDec>
    <keyword> function </keyword>
    <keyword> void </keyword>
    <identifier> main </identifier>
    <symbol> ( </symbol>
    <parameterList>
    </parameterList>
    <symbol> ) </symbol>
    <subroutineBody>
      <symbol> { </symbol>
      <varDec>
        <keyword> var </keyword>
        <identifier> SquareGame </identifier>
        <identifier> game </identifier>
        <symbol> ; </symbol>
      </varDec>
      <statements>
        <letStatement>
          <keyword> let </keyword>
          <identifier> game </identifier>
          <symbol> = </symbol>
          <expression>
            <term>
              <identifier> SquareGame </identifier>
              <symbol> . </symbol>
              <identifier> new </identifier>
              <symbol> ( </symbol>
              <expressionList>
              </expressionList>
              <symbol> ) </symbol>
            </term>
          </expression>
          <symbol> ; </symbol>
        </letStatement>
        <doStatement>
          <keyword> do </keyword>
          <identifier> game </identifier>
          <symbol> . </symbol>
          <identifier> run </identifier>
          <symbol> ( </symbol>
          <expressionList>
          </expressionList>
          <symbol> ) </symbol>
          <symbol> ; </symbol>
        </doStatement>
        <doStatement>
          <keyword> do </keyword>
          <identifier> game </identifier>
          <symbol> . </symbol>
          <identifier> dispose </identifier>
          <symbol> ( </symbol>
          <expressionList>
          </expressionList>
          <symbol> ) </symbol>
          <symbol> ; </symbol>
        </doStatement>
        <returnStatement>
          <keyword> return </keyword>
          <symbol> ; </symbol>
        </returnStatement>
      </statements>
      <symbol> } </symbol>
    </subroutineBody>
  </subroutineDec>
  <symbol> } </symbol>
</class>
```
