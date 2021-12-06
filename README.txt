Student: Sean Dela Pena 
Professor: Mark Hauschild
Assignment: Project 5 - CS 4280
	Code Genration
Date: 11/28/21

OPTION 2 (LOCAL OPTION) (max 100):
	Variables defined before the program keyword are considered in the global scope, those inside of any block are considered scoped in that block.

USAGE: make		(makefile to compile, creates a parser executable)
	./compfs [somefile]
	make clean	

Invocation:
	./compfs somefile.txt [enter]


Any other invocations will result in an error.

Notes:
	The token structure is as shown
	[token, "string", line num:charNum]
	["IDTk, "example", line 1:1]

	The Parser error structure is as shown
	Parser Error: There is an error!
	Parser Error: ERROR in tk [tokenthatwaschecked] expected [expectedToken] in line [lineNum]

	The Semantics error structure is as shown
	Semantic Error: This variable "var" has ALREADY been declared.
	or
	Semantic Error: This variable "var" has NOT been declared.

Important:
	
