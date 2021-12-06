//Sean Dela Pena
//FILE: node.hpp
//

#ifndef NODE_HPP
#define NODE_HPP
#include"token.hpp"

struct Node {

	//stores the tokens that is consumed
	token token1;
	token token2;

	//5 max children
	Node* child1;
	Node* child2;
	Node* child3;
	Node* child4;
	Node* child5;

	//enum for the nodeType/fuction that is called
	int funcCalled;
};

#endif
