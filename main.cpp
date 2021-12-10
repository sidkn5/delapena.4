/*Student: Sean Dela Pena
 *Assignment P4 - Code Generator
 *Professor: Mark Hauschild
 *Class: CS 4280
 *Date: 12-10-21
 * 
 * File: main.cpp
 * This file handles the arguments, calls the scanner, calls the parser, calls getTree which sets the 
 * tree and then calls semantic() to check the semantics of the tree. Then it calls the generator which 
 * which generates the machine code into a target file.
 */

#include "scanner.hpp"
#include "parser.hpp"
#include "tree.hpp"
#include "semantic.hpp"
#include "codegenerator.hpp"


int main(int argc, char* argv[]){

	std::string filename = "";
	std::string asmFilename = "";
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
			
			//change to asm later
			asmFilename = filename.append(".asm");
			generator(root, filename);

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
