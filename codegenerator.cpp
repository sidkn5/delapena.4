//Student: Sean Dela Pena
//File: codegenerator.cpp
//
#include "codegenerator.hpp"

static int LabelCntr = 0; /* counting unique labels generated */
static int NewVarCntr = 0; /* counting unique temporaries generated */
int varInt = 0;				//pass to gen check
std::ofstream targetFile;

//the stack is in a vector that contains the token and it's scope
std::vector<tokenStack> varStack;
static int numOfGlobals = 0;

//used for newNames
typedef enum {
	VAR, LABEL
} nameType;

//used for scoping
enum scope {
	GLOBALSCOPE, LOCALSCOPE
};


//good
//main driver of code generation
void generator(Node* node, std::string filename) {
	
	//make a target file
	targetFile.open(filename);

	if (targetFile) {
		std::cout << filename <<" created ... \n";

	}
	else {
		std::cout << "There is an error with the target file.\n";
	}

	//just check the if the parse tree is empty or invalid
	if (node == NULL || node->funcCalled != PROGRAMNODE) {
		std::cout << "CODEGEN ERROR: The parse tree is either NULL or invalid...\n";
		exit(1);
	}

	//get the globals
	getGlobals(node->child1);
	generatorCheck(node->child2, varInt);

	//end of the file
	targetFile << "STOP\n";
	//print the globals
	for (int i = 0; i < varStack.size(); i++) {
		if (varStack.at(i).varScope == GLOBALSCOPE) {
			targetFile << varStack.at(i).tk.tokenString << " 0\n";
		}
	}

	//print the new names
	for (int i = 0; i < NewVarCntr; i++) {
		targetFile << "T" << i << " 0 \n";
 	}
	
	targetFile.close();
}


//takes care of the block node
void generatorCheck(Node *node, int &VarCntr) {
	if (node == NULL) {
		return;
	}
	if (node->funcCalled == BLOCKNODE) {
		int localVarCount = 0;
		getLocals(node, localVarCount);
		generatorCheck(node->child2, localVarCount);
		
		popStack(localVarCount);
		
	}
	else {
		recGen(node, VarCntr);
	}
}

//gets the local variables
void getLocals(Node* node, int& VarCntr) {

	if (node == NULL) {
		return;
	}
	
	if (node->token1.type != WSTK && node->token1.type != HOLDERTK) {
		
		//push the token to the stack with a LOCAL SCOPE
		push(node->token1, LOCALSCOPE);
		targetFile << "LOAD " << node->token2.tokenString << std::endl;
		targetFile << "PUSH \n";
		targetFile << "STACKW 0\n";
		VarCntr++;
	}
	getLocals(node->child1, VarCntr);
}

//get the gloabl variables
//load and store the globals
void getGlobals(Node* node) {
	if (node == NULL) {
		return;
	}
	if (node->token1.type != WSTK && node->token1.type != HOLDERTK){
		//push the token to the stack with a global scope
		push(node->token1, GLOBALSCOPE);
		numOfGlobals++;		//count the number of globals in the stack
		targetFile << "LOAD " << node->token2.tokenString << "\n";
		targetFile << "STORE " << node->token1.tokenString << "\n";
	}
	getGlobals(node->child1);
}


void recGen(Node* node, int& VarCntr) {
	
	if (node == NULL) {
		return;
	}

	switch (node->funcCalled) {
		
		case EXPRNODE:
			if (node->token1.type != WSTK && node->token1.type != HOLDERTK) {
				//call new name
				std::string newArg;
				newArg = newName(VAR);
				recGen(node->child2, VarCntr);

				targetFile << "STORE " << newArg << "\n";
				recGen(node->child1, VarCntr);
				if (node->token1.type == PLUSTK) {
					targetFile << "ADD " << newArg << "\n";
				}
				
			}
			else {
				recGen(node->child1, VarCntr);
			}
			break;


		case ANODE: {
			if (node->token1.type != WSTK && node->token1.type != HOLDERTK) {
				std::string newArg;
				newArg = newName(VAR);
				recGen(node->child2, VarCntr);
				targetFile << "STORE " << newArg << "\n";
				if (node->token1.type == MINUSTK) {
					targetFile << "SUB " << newArg << "\n";
				}
				
			}
			else {
				recGen(node->child2, VarCntr);
			}

			break;

		}
		// N -> A / N | A * N | A
		case NNODE: {
			
			if (node->token1.type != WSTK && node->token1.type != HOLDERTK) {
				std::string newArg;
				newArg = newName(VAR);
				recGen(node->child2, VarCntr);

				
				targetFile << "STORE " << newArg << "\n";
				recGen(node->child1, VarCntr);
				if (node->token1.type == MULTIPLYTK) {
					targetFile << "MULT " << newArg << "\n";
				}
				else if (node->token1.type == DIVIDETK) {
					targetFile << "DIV " << newArg << "\n";
				}
			}
			else {
				recGen(node->child1, VarCntr);
			}

			break;
		}

		
		case MNODE: 
			recGen(node->child1, VarCntr);
			if (node->token1.type != WSTK && node->token1.type != HOLDERTK) {
				if (node->token1.type == DOTTK) {
					targetFile << "MULT -1 \n";
				}

			}
			break;

		case RNODE: 
			//printStack();
			if (node->token1.type != WSTK && node->token1.type != HOLDERTK) {
				if (node->child1 == NULL) {


					if (node->token1.type == IDTK) {
						if (findVar(node->token1) != -1) {
							targetFile << "STACKR " << findVar(node->token1) << "\n";

						}
						else {
							targetFile << "LOAD " << node->token1.tokenString << "\n";
							
						}
					}
					else if (node->token1.type == INTEGERTK) {
						targetFile << "LOAD " << node->token1.tokenString << "\n";
					}

				}
				else {
					recGen(node->child1, VarCntr);
				}

			}
			break;


		case INNODE: {
			std::string newArg;
			newArg = newName(VAR);

			if (findVar(node->token1) != -1) {
				targetFile << "READ " << newArg << "\n";
				targetFile << "LOAD " << newArg << "\n";
				targetFile << "STACKW " << findVar(node->token1) << "\n";
				
			}else {
				targetFile << "READ " << node->token1.tokenString << "\n";
			}
			
			break;
		}

		case OUTNODE: {
			std::string newArg;
			recGen(node->child1, VarCntr);
			newArg = newName(VAR);
			targetFile << "STORE " << newArg << "\n";
			targetFile << "WRITE " << newArg << "\n";

			break;
		}
			


		// assign ID = <expr>
		case ASSIGNNODE:
			if (findVar(node->token1) != -1) {
				recGen(node->child1, VarCntr);
				targetFile << "STACKW " << findVar(node->token1) << "\n";
			}
			else {
				recGen(node->child1, VarCntr);
				targetFile << "STORE " << node->token1.tokenString << "\n";
			}
			
			break;

		//jump ID
		case GOTOFUNCNODE:
			if (node->token1.type != WSTK && node->token1.type != HOLDERTK) {
				if(node->token1.type == IDTK)
					targetFile << "BR " << node->token1.tokenString << "\n";
			
			}
			break;

		//label ID
		case LABELNODE:
			if (node->token1.type != WSTK && node->token1.type != HOLDERTK) {
				if (node->token1.type == IDTK)
					targetFile << node->token1.tokenString << ": NOOP \n";
					
			}
			break;

		//travers left first, then right, then the rest of the children.
		case IFSTATNODE: {
			std::string newArg;
			std::string newLabel;
			std::string anotherLabel;

			newArg = newName(VAR);
			newLabel = newName(LABEL);
			anotherLabel = newName(LABEL);

			if (node->child2->token1.type == MODULUSTK) {

				//hits when we get a % op, multiply the arguments if neg then false
				//traverse right left
				generatorCheck(node->child3, VarCntr);
				targetFile << "STORE " << newArg << "\n";
				generatorCheck(node->child1, VarCntr);
				targetFile << "MULT " << newArg << "\n";
				targetFile << "BRNEG " << newLabel << "\n";

				generatorCheck(node->child4, VarCntr);
				targetFile << "BR " << anotherLabel << "\n";
				targetFile << newLabel << ": NOOP" << "\n";

				generatorCheck(node->child5, VarCntr);
				targetFile << anotherLabel << ": NOOP\n";
			}
			else {

				generatorCheck(node->child3, VarCntr);
				targetFile << "STORE " << newArg << "\n";

				//newArg = newName(VAR);
				generatorCheck(node->child1, VarCntr);
				targetFile << "SUB " << newArg << "\n";


				// check the RO
				//token2 holds the braces so if token2 is not a right brace then it is the comparison == op,
				//otherwise it is the { == } op
				if (node->child2->token1.type == EQEQTK && node->child2->token2.type != RIGHTBRACETK) {
					targetFile << "BRPOS " << newLabel << "\n";
					targetFile << "BRNEG " << newLabel << "\n";
				}
				else if (node->child2->token1.type == GREATERTHANTK) {
					targetFile << "BRZNEG " << newLabel << "\n";
				}
				else if (node->child2->token1.type == LESSTHANTK) {
					targetFile << "BRZPOS " << newLabel << "\n";
				}
				else {
					targetFile << "BRZERO " << newLabel << "\n";
				}

				generatorCheck(node->child4, VarCntr);


				targetFile << "BR " << anotherLabel << "\n";
				targetFile << newLabel << ": NOOP" << "\n";

				generatorCheck(node->child5, VarCntr);

				targetFile << anotherLabel << ": NOOP" << "\n";


			}

			break;
		}

			

		//traverse right first then left, then the rest of children
		case LOOPNODE: {
			std::string newArg;
			std::string newLabel;
			std::string anotherLabel;

			newArg = newName(VAR);
			newLabel = newName(LABEL);
			anotherLabel = newName(LABEL);


			if (node->child2->token1.type == MODULUSTK) {

				//hits when we get a % op, multiply the arguments if neg then false
				//traverse right left
				targetFile << newLabel << ": NOOP" << "\n";
				generatorCheck(node->child3, VarCntr);
				targetFile << "STORE " << newArg << "\n";
				generatorCheck(node->child1, VarCntr);
				targetFile << "MULT " << newArg << "\n";
				targetFile << "BRNEG " << anotherLabel << "\n";

				generatorCheck(node->child4, VarCntr);
				targetFile << "BR " << newLabel << "\n";
				targetFile << anotherLabel << ": NOOP" << "\n";


			}
			else {
				targetFile << newLabel << ": NOOP" << "\n";

				generatorCheck(node->child3, VarCntr);
				targetFile << "STORE " << newArg << "\n";

				//newArg = newName(VAR);
				generatorCheck(node->child1, VarCntr);
				targetFile << "SUB " << newArg << "\n";

				// check the RO
				//token2 holds the braces so if token2 is not a right brace then it is the comparison ==
				if (node->child2->token1.type == EQEQTK && node->child2->token2.type != RIGHTBRACETK) {
					targetFile << "BRPOS " << anotherLabel << "\n" << "BRNEG " << anotherLabel << "\n";
				}
				else if (node->child2->token1.type == GREATERTHANTK) {
					targetFile << "BRZNEG " << anotherLabel << "\n";
				}
				else if (node->child2->token1.type == LESSTHANTK) {
					targetFile << "BRZPOS " << anotherLabel << "\n";
				}
				else {
					targetFile << "BRZERO " << anotherLabel << "\n";
				}

				recGen(node->child4, VarCntr);
				targetFile << "BR " << newLabel << "\n";
				targetFile <<  anotherLabel << ": NOOP\n";

			}

			break;
		}

			
		//traverses other cases not handled above
		default:
			recGen(node->child1, VarCntr);
			recGen(node->child2, VarCntr);
			recGen(node->child3, VarCntr);
			recGen(node->child4, VarCntr);
			recGen(node->child5, VarCntr);
			break;

	}
}

//find if the element is on the stack, and return the position with in the stack from the head/top
int findVar(token toke) {
	int posInStack = 0;

	for (int i = varStack.size() - 1; i >=0; i--) {
		if (varStack.at(i).varScope == LOCALSCOPE) {
			if (varStack.at(i).tk.tokenString == toke.tokenString) {
				return posInStack;
			}
			posInStack++;
		}
	}


	return -1;
}


//create a new variable
std::string newName(int newVar)
{
	std::string Name = "";
	if (newVar == VAR) {
		Name = "T" + std::to_string(NewVarCntr++);
		
	}
	else {
		Name = "L" + std::to_string(LabelCntr++);
	}

	return(Name);
}

//pop from the stack after a blocknd
void popStack(int n) {
	for (int i = 0; i < n; i++) {
		targetFile << "POP \n";
		varStack.pop_back();
	}
}


//set the token and scope to be pushed into the stack/vector
void push(token toke, int scope) {
	//std::cout << "Pushing variable " << str << std::endl;
	tokenStack addToken;
	addToken.tk = toke;
	addToken.varScope = scope;
	varStack.push_back(addToken);
	return;
}
/*
void printStack() {
	for (int i = 0; i < localVars.size(); i++) {
		targetFile << localVars.at(i).tokenString << " \n";
	}
	std::cout << "\n";
}*/


