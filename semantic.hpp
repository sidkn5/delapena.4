//Sean Dela Pena
//File: semantic.hpp
//
// Assignment P3 - Semantics
// Professor: Mark Hauschild
// Class : CS 4280
// Date : 11 - 28 - 21

#ifndef STATIC_CPP
#define STATIC_CPP

#include <vector>
#include <string>
#include "token.hpp"
#include "tree.hpp"
#include "node.hpp"
#include "character.hpp"

void push(std::string varString, int varScope);
void pop();
int find(std::string varString, int varScope);
void getTree(Node* node);
void semantic(Node *node);
void getVariablesLeft(Node* node);
void getVariablesRight(Node* node, int &varCount);
void getBlock(Node* node);
void preOrder(Node* node);
void error(std::string var);
void error2(std::string str);


enum scope {
	GLOBALSCOPE, LOCALSCOPE
};


struct variable {
	std::string varString;
	int varScope;
};


#endif 
