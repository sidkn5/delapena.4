//Sean Dela Pena
//File: tree.hpp
//

#ifndef TREE_HPP
#define TREE_HPP
#include <iostream>
#include "node.hpp"

enum nodeType {
	PROGRAMNODE, BLOCKNODE, VARSNODE, EXPRNODE, NNODE, ANODE, MNODE,
	RNODE, STATSNODE, MSTATNODE, STATNODE, INNODE, OUTNODE, IFSTATNODE,
	LOOPNODE, ASSIGNNODE, RONODE,LABELNODE, GOTOFUNCNODE
};

//
Node* getNode(int nodeType);
void printNode(Node* node, int depth);

#endif
