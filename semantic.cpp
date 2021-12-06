//Sean Dela Pena
//File: semantic.cpp
//
// Assignment P3 - Semantics
// Professor: Mark Hauschild
// Class : CS 4280
// Date : 11 - 28 - 21

#include <vector>
#include <string>
#include "semantic.hpp"

//variable contains the var string, and an int scope
std::vector<variable> variableStack;
Node* root = NULL;

//sets the tree that was made by the parser
void getTree(Node* node) {
	//set tree
	root = node;
	semantic(root);
}

//traverse tree in preorder (root, left, right)
void semantic(Node *node) {

	if (node == NULL) {
		std::cout << "SEMANTIC ERROR: Tree is empty...\n";
		exit(1);
	}
	
	//Get the gloabal variables, anything before program root is a global
	getVariablesLeft(node->child1);
	//check the right side of program, locals
	getBlock(node->child2);

	variableStack.clear();		//clears the globals left in the stack
}

//set the string and the scope of element then push to vector/stack
void push(std::string str, int scope) {
	//std::cout << "Pushing variable " << str << std::endl;
	variable newVariable;
	newVariable.varString = str;
	newVariable.varScope = scope;
	variableStack.push_back(newVariable);
	return;
}

//pop the very end of the vector, similar to popping the top of a stack
void pop() {
	//std::cout << "Popping variable " << std::endl;
	variableStack.pop_back();
	return;
}

//find if the element is on the stack, checks for the scope as well
int find(std::string varString, int varScope) {
	
	for (int i = 0; i < variableStack.size(); i++) {
		
		//check if the variable is in the stack, matching with it's scope
		if (variableStack.at(i).varString == varString && variableStack.at(i).varScope == varScope) {
			//std::cout<< "Variable " << varString << " found in stack..\n";
			return 1;
		}
	}

	return -1;
}


//goes to the left of the tree and grabs the global vars
void getVariablesLeft(Node* node) {

	//if id, work with it
	if (node->token1.type == IDTK) {

		if (find(node->token1.tokenString, GLOBALSCOPE) != -1) {
			//the id is already in the vector/stack, we're dealing with globals
			error2(node->token1.tokenString);
		}
		else {
			//add the id to the vector/stack
			push(node->token1.tokenString, GLOBALSCOPE);
		}
		if (node->child1 != NULL) {
			//recur
			getVariablesLeft(node->child1);
		}
	}
	else {
		return;
	}

}

//goes to right of the tree and grabs the local vars
void getVariablesRight(Node* node, int &varCount) {

	//if id, work with it
	if (node->token1.type == IDTK) {
	
		if (find(node->token1.tokenString, LOCALSCOPE) != -1) {
			//the id is already in the vector/stack, we're dealing with locals
			error2(node->token1.tokenString);
		}
		else {
			//add the id to the vector/stack
			push(node->token1.tokenString, LOCALSCOPE);
			varCount++;
		}
		if (node->child1 != NULL) {
			//recur
			getVariablesRight(node->child1, varCount);
		}
	}
	else {
		return;
	}

}

//traversing the block node
void getBlock(Node* node) {

	int varCount = 0;	//init a varcount to 0 when you get a block
	getVariablesRight(node->child1, varCount);		//get the locals in the block node
	if (node->child2 == NULL) {
		//skip the child nothing in it
	}
	else {
		preOrder(node->child2);
	}

	//pop depending on the varCount
	for (int i = 0; i < varCount; i++) {
		pop();
	}
}

void preOrder(Node* node) {

	//funcCalled is just the node type
	if (node->funcCalled != BLOCKNODE) {
		//check the tokens in the node

		if (node->token1.type == IDTK) {
			
			//check if the string is in the global scope
			if (find(node->token1.tokenString, GLOBALSCOPE) != -1) {
				//found in global stack
			}
			//check if the string is in the local scope
			else if (find(node->token1.tokenString, LOCALSCOPE) != -1) {
				//found in local stack
			}
			else {
				//not found
				error(node->token1.tokenString);
			}
		}
		if (node->token2.type == IDTK) {
			
			//check if the string is in the global scope
			if (find(node->token2.tokenString, GLOBALSCOPE) != -1) {
				//found in global stack
			}
			//check if the string is in the local scope
			else if (find(node->token2.tokenString, LOCALSCOPE) != -1) {
				//found in local stack
			}
			else {
				error(node->token2.tokenString);
			}
		}

		//visit the children
		if (node->child1 != NULL)
			preOrder(node->child1);

		if (node->child2 != NULL)
			preOrder(node->child2);

		if (node->child3 != NULL)
			preOrder(node->child3);

		if (node->child4 != NULL)
			preOrder(node->child4);

		if (node->child5 != NULL)
			preOrder(node->child5);
	}
	//funcCalled is just the node type
	else if (node->funcCalled == BLOCKNODE){
		//new block found
		getBlock(node);
	}
}

void error(std::string var) {
	std::cout << "SEMANTIC ERROR: This variable \"" << var << "\" has NOT been declared. \n";
	exit(1);
}

void error2(std::string str) {
	std::cout << "SEMANTIC ERROR: This variable \"" << str << "\" has ALREADY been declared. \n";
	exit(1);
}








