/*Student: Sean Dela Pena
 *Assignment P3 - Semantics
 *Professor: Mark Hauschild
 *Class: CS 4280
 *Date: 11-28-21
 * 
 * File: main.cpp
 * This file handles the arguments, calls the scanner, calls the parser, calls getTree which sets the 
 * tree and then calls semantic() to check the semantics of the tree.
 */

#include "scanner.hpp"
#include "parser.hpp"
#include "tree.hpp"
#include "semantic.hpp"

int main(int argc, char* argv[]){

	std::string filename = "";
	std::ifstream file;
	
	if (argc > 2) {
		printf("ERROR: Incorrect use of the program.\n");
		exit(0);
	}
	//filename given
	else if (argc == 2) {
		filename = argv[1];
		file.open(filename);
		if (file) {
			std::cout << "Opening and working with the file...\n";
			
			getAllTokens(filename);
			Node* root = parser();
			//printNode(root, 0);
			getTree(root);
			
			std::cout << "Semantics Passed/Completed...\n";
		}
		else {
			std::cout << "The file does not exist! Terminating...\n";
			exit(0);
		}
	}
	else {
		printf("ERROR: Incorrect use of the program.\n");
		exit(0);
	}

	file.close();

	return 0;
}
