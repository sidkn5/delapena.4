//Student: Sean Dela Pena
//File: codegenerator.hpp


#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP
#include "scanner.hpp"
#include "token.hpp"
#include "node.hpp"
#include "tree.hpp"
#include "parser.hpp"

void generator(Node* node, std::string filename);
void generatorCheck(Node* node, int& VarCntr);
void getLocals(Node* node, int& VarCntr);
void getGlobals(Node* node);
void recGen(Node* node, int& VarCntr);
int findVar(token toke);
std::string newName(int newVar);
void popStack(int n);
void printStack();
void push(token tk, int scope);


struct tokenStack {
	token tk;
	int varScope;
};


#endif