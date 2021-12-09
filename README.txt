Student: Sean Dela Pena 
Professor: Mark Hauschild
Assignment: Project 4 - CS 4280
	Code Genration
Date: 12/09/21

LOCAL OPTION (max 130 + 20):
	P3 and P4 are both LOCAL options in this project. 

USAGE: make		(makefile to compile, creates an executable)
	./compfs [somefile]
	make clean	

Invocation:
	./compfs somefile.txt [enter]


Any other invocations will result in an error.

Notes:
	Will produce a "[somefile].asm" file to be ran in the virtual machine.

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
	The main.cpp file calls the scanner, the parser, the semantic, then the generator to produce
	a file named "[somefile].asm" which can be ran in the virtual machine. I decide to leave the sematic
	check since it doesn't really hurt the program. The generator will still do it's own pass/traversing
	after the semantic check. 
