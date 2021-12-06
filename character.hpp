// Student: Sean Dela Pena
// File: character.hpp
//


#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include<sstream>
#include<map>

struct LinesContainer{
	std::string value;
	int columnNum;
	int lineNum;
	bool endOfFile = false;

};


enum charType{
	LOWERCASE, UPPERCASE, INTEGER, DOLLAR, WS, EQUAL, GREATERTHAN, LESSTHAN, COLON,
	PLUS, MINUS, MULTIPLY, DIVIDE, MODULUS, DOT, LEFTPAREN, RIGHTPAREN,
	COMMA, LEFTBRACE, RIGHTBRACE, SEMICOLON, LEFTBRACKET, RIGHTBRACKET
};



#endif