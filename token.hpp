// Student: Sean Dela Pena
// token.hpp
//


#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <string>

enum tokenType {
	//identifier, integer and eof
	HOLDERTK, IDTK, INTEGERTK, WSTK, 

	//operators and delimiters
	EQUALTK, EQEQTK, GREATERTHANTK, LESSTHANTK, COLONTK,
	COLONEQTK, PLUSTK, MINUSTK, MULTIPLYTK, DIVIDETK, MODULUSTK, DOTTK, LEFTPARENTK, RIGHTPARENTK,
	COMMATK, LEFTBRACETK, RIGHTBRACETK, SEMICOLONTK, LEFTBRACKETTK, RIGHTBRACKETTK,
	EOFTK,
	//reserved keywords
	KEYWORDTK, 
	STARTTK, STOPTK, LOOPTK, WHILETK, FORTK, LABELTK, EXITTK, LISTENTK, TALKTK,
	PROGRAMTK, IFTK, THENTK, ASSIGNTK, DECLARETK, JUMPTK, ELSETK, RELATIONALOPERATORTK
	
};

struct token{
	tokenType type;
	std::string tokenString;
	int lineNum;
	int charNum;
};


#endif