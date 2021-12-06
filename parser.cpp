//Sean Dela Pena
//File: parser.cpp
//
// Assignment P3 - Semantics
// Professor: Mark Hauschild
// Class : CS 4280
// Date : 11 - 28 - 21

#include "parser.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include "node.hpp"

static int index = -1;			//index used to pop from the vector one at a time
static token tk;
std::string filename;
std::vector<token> allTokens;

//will be used for errors, expected tokens
std::string expectedTokens[] = {
	"HolderTk", "IDTk", "INTEGERTk", "WSTk", "EQUALTk", "EQEQTk", "GREATERTHANTk", "LESSTHANTk", "COLONTk",
	"COLONEQTk", "PLUSTk", "MINUSTk", "MULTIPLYTk", "DIVIDETk", "MODULUSTk", "DOTTk", "LEFTPARENTK",
	"RIGHTPARENTk", "COMMATk", "LEFTBRACETk", "RIGHTBRACETk", "SEMICOLONTk", "LEFTBRACKETTk", "RIGHTBRACKETTk",
	"EOFTk", "KEYWORDTk", "STARTTk", "STOPTk", "LOOPTk", "WHILETk", "FORTk", "LABELTk", "EXITTk", "LISTENTk",
	"TALKTk", "PROGRAMTk", "IFTk", "THENTk", "ASSIGNTk", "DECLARETk", "JUMPTk", "ELSETk", "RELATIONALOPERATORTk"
};

void getAllTokens(std::string filename) {
	allTokens = testScanner(filename);
}

//pop from the vector of tokens one at a time
token scanner() {
	index++;
	//return tks.at(index);

	//std::cout << "[" << expectedTokens[allTokens[index].type] << ", \"" << allTokens[index].tokenString
		//<< "\", line " << allTokens[index].lineNum << ":" << allTokens[index].charNum << "]" << std::endl;


	return allTokens.at(index);
}

Node* parser() {

	Node* root;

	tk = scanner();
	root = program();
	if (tk.type == EOFTK) {
		//std::cout << "Parser Completed/Passed\n";
		//exit(1);
		//end
	}

	return root;

}

/***********************     GRAMMAR FUNCTIONS    ***********************************/

// <vars> program <block>
Node* program() {
	
	//std::cout << "Program called \n";
	Node* node = getNode(PROGRAMNODE);
	node->child1 = vars();

	if (tk.type == PROGRAMTK) {
		tk = scanner();
		node->child2 = block();
	}
	else {
		error(tk, PROGRAMTK);
	}
	return node;
}

//start <vars> <stats> stop
Node* block() {

	Node* node = getNode(BLOCKNODE);
	//std::cout << "block called \n";

	if (tk.type == STARTTK) {
		tk = scanner();
		node->child1 = vars();
		node->child2 = stats();
		if (tk.type == STOPTK) {
			tk = scanner();
			return node;
		}
		else {
			error(tk, STOPTK);
		}
	}
	else {
		error(tk, STARTTK);		//expected start token
	}

	return NULL;
}


// empty | declare Identifier =  Integer  ;  <vars> 
Node* vars() {
	//std::cout << "vars called \n";

	Node* node = getNode(VARSNODE);

	if (tk.type == DECLARETK) {
		tk = scanner();

		if (tk.type == IDTK) {
			node->token1 = tk;			//consume IDTK
			tk = scanner();

			if (tk.type == EQUALTK) {
				tk = scanner();

				if (tk.type == INTEGERTK) {
					node->token2 = tk;		//consume INTEGERTK
					tk = scanner();

					if (tk.type == SEMICOLONTK) {
						tk = scanner();
						node->child1 = vars();
						return node;
					}
					else {
						error(tk, SEMICOLONTK);	//expected semicolon token
					}
				}
				else {
					error(tk, INTEGERTK);	//expected integer token
				}
			}
			else {
				error(tk, EQUALTK);		//expected equal  token
			}
		}
		else {
			error(tk, IDTK);	//expected identifier token
		}
	}
	else {
		return node;	//vars can be empty
	}

	return node;
}

//<expr> -> <N> + <expr>  | <N>
Node* expr() {

	Node* node = getNode(EXPRNODE);
	//std::cout << "expr called \n";
	node->child1 = N();
	if (tk.type == PLUSTK) {
		node->token1 = tk;
		tk = scanner();
		node->child2 = expr();

	}
	else {
		return node;
	}
	return node;

}

//<N> -> <A> / <N> | <A> *<N> | <A>
Node* N() {
	//std::cout << "N called \n";

	Node* node = getNode(NNODE);
	node->child1 = A();

	if (tk.type == DIVIDETK || tk.type == MULTIPLYTK) {
		node->token1 = tk;			//consume operator
		tk = scanner();
		node->child2 = N();
	}
	else {
		return node;
	}

	return node;
}


//<A> -> <M>-<A> | <M>
Node* A() {
	//std::cout << "A called \n";
	Node* node = getNode(ANODE);
	node->child1 = M();

	if (tk.type == MINUSTK) {
		node->token1 = tk;	//consume operator
		tk = scanner();
		node->child2 = A();
	}
	else {
		return node;
	}

	return node;
}

//<M> -> . <M> | <R>
Node* M() {

	Node* node = getNode(MNODE);
	if (tk.type == DOTTK) {
		node->token1 = tk;			//consume operator
		tk = scanner();
		node->child1 = M();
		return node;
	}
	else {
		node->child1 = R();
		return node;
	}

	return node;
}


//<R> -> ( <expr> ) | Identifier | Integer
Node* R() {

	Node* node = getNode(RNODE);
	if (tk.type == LEFTPARENTK) {
		tk = scanner();
		node->child1 = expr();

		if (tk.type == RIGHTPARENTK) {
			tk = scanner();
		}
		else {
			error(tk, RIGHTPARENTK);
		}
	}
	else if (tk.type == IDTK) {
		node->token1 = tk;		//consume operator
		tk = scanner();
		return node;
	}
	else if (tk.type == INTEGERTK) {
		node->token1 = tk;		//consume operator
		tk = scanner();
		return node;
	}
	else {
		return node;
	}

	return NULL;
}

//<stats> -> <stat>  <mStat>
Node* stats() {
	//std::cout << "stats called \n";

	Node* node = getNode(STATSNODE);
	node->child1 = stat();
	node->child2 = mStat();

	return node;
}

//<mStat> -> empty |  <stat>  <mStat>
Node* mStat() {
	
	//std::cout << "mstat called \n";

	Node* node = getNode(MSTATNODE);
	if (tk.type == LISTENTK || tk.type == TALKTK || tk.type == STARTTK || tk.type == IFTK || tk.type == WHILETK 
		|| tk.type == ASSIGNTK || tk.type == JUMPTK || tk.type == LABELTK) {
		node->child1 = stat();
		node->child2 = mStat();

		return node;
	}
	else {
		//mstat can be empty
		return node;
	}

}


Node* stat() {
	//std::cout << "stat called \n";

	Node* node = getNode(STATNODE);
	if (tk.type == LISTENTK) {
		tk = scanner();
		node->child1 = in();

		if (tk.type == SEMICOLONTK) {
			tk = scanner();
			
			return node;
		}
		else {
			error(tk, SEMICOLONTK);		//expected semicolon token
			return NULL;
		}
	}
	else if (tk.type == TALKTK) {
		tk = scanner();
		node->child1 = out();

		if (tk.type == SEMICOLONTK) {
			tk = scanner();

			return node;
		}
		else {
			error(tk, SEMICOLONTK);		//expected semicolon token
		}
	} 
	else if (tk.type == STARTTK) {
	
		node->child1 = block();
		return node;

		//don't check for semicolon

	}
	else if (tk.type == IFTK) {
		tk = scanner();
		node->child1 = ifStat();

		if (tk.type == SEMICOLONTK) {
			tk = scanner();

			return node;
		}
		else {
			error(tk, SEMICOLONTK);		//expected semicolon token
			return NULL;
		}
	}
	else if (tk.type == WHILETK) {
		tk = scanner();
		node->child1 = loop();

		if (tk.type == SEMICOLONTK) {
			tk = scanner();

			return node;
		}
		else {
			error(tk, SEMICOLONTK);		//expected semicolon token
			return NULL;
		}
	}
	else if (tk.type == ASSIGNTK) {
		tk = scanner();
		node->child1 = assign();

		if (tk.type == SEMICOLONTK) {
			tk = scanner();

			return node;
		}
		else {
			error(tk, SEMICOLONTK);		//expected semicolon token
			return NULL;
		}
	}
	else if (tk.type == JUMPTK) {
		tk = scanner();
		node->child1 = gotoFunc();

		if (tk.type == SEMICOLONTK) {
			tk = scanner();
			
			return node;
		}
		else {
			error(tk, SEMICOLONTK);		//expected semicolon token
			return NULL;
		}
	}
	else if (tk.type == LABELTK) {
		tk = scanner();
		node->child1 = label();

		if (tk.type == SEMICOLONTK) {
			tk = scanner();
			
			return node;
		}
		else {
			error(tk, SEMICOLONTK);		//expected semicolon token
			return NULL;
		}
	}
	else {
		//should not be able to reach here
		return NULL;
	}

	return NULL;
}

//<in>->listen  Identifier
Node* in() {
	//std::cout << "in called \n";

	Node* node = getNode(INNODE);
	
	if (tk.type == IDTK) {
		node->token1 = tk;		//consume ID token
		tk = scanner();
		return node;
	}
	else {
		error(tk, IDTK);		//expected ID token
		return NULL;
	}
}


//<out> -> talk <expr>
Node* out() {
	//std::cout << "out called\n";
	Node* node = getNode(OUTNODE);
	
	node->child1 = expr();
	return node;
}


//hits 5 children
Node* ifStat() {

	Node* node = getNode(IFSTATNODE);
	if (tk.type == LEFTBRACKETTK) {
		tk = scanner();

		node->child1 = expr();
		node->child2 = RO();
		node->child3 = expr();

		if (tk.type == RIGHTBRACKETTK) {
			tk = scanner();
			
			if (tk.type == THENTK) {
				tk = scanner();
				node->child4 = stat();

				if (tk.type == ELSETK) {
					tk = scanner();
					node->child5 = stat();
				}
				return node;
			}
			else {
				error(tk, THENTK);		//expected THENTK
				return NULL;
			}
		}
		else {
			error(tk, RIGHTBRACKETTK);	//expected RIGHTBRACKET token
			return NULL;
		}

	}
	else {
		error(tk, LEFTBRACKETTK);		//expected LEFTBRACKET token
		return NULL;
	}

}


// <loop> -> while[<expr> <RO> <expr>]  <stat>
Node* loop() {
	//std::cout << "loop called \n";
	Node* node = getNode(LOOPNODE);

	if (tk.type == LEFTBRACKETTK) {
		tk = scanner();
		node->child1 = expr();
		node->child2 = RO();
		node->child3 = expr();
		if (tk.type == RIGHTBRACKETTK) {
			tk = scanner();
			node->child4 = stat();
			return node;
		}
		else {
			error(tk, RIGHTBRACKET);		//expected RIGHTBRACKET token
			return NULL;
		}
	}
	else {
		error(tk, LEFTBRACKET);				//expected LEFTBRACKET token
		return NULL;
	}

	return NULL;
}


//<assign> -> assign Identifier  = <expr>  
Node* assign() {
	//std::cout << "assign called \n";

	Node* node = getNode(ASSIGNNODE);
	if (tk.type == IDTK) {
		node->token1 = tk;			//consume ID token
		tk = scanner();

		if (tk.type == EQUALTK) {
			tk = scanner();
			node->child1 = expr();

		}
		else {
			error(tk, EQUALTK);		//expected EQUAL token
		}
	}
	else {
		error(tk, IDTK);		//expected ID token
	}

	return NULL;

}


//<RO> -> > | < | == | { == }  (three tokens) | %
Node* RO() {
	//std::cout << "RO called\n";
	Node* node = getNode(RONODE);

	if (tk.type == GREATERTHANTK) {
		node->token1 = tk;			//consume operation
		tk = scanner();

		return node;
	}
	else if (tk.type == LESSTHANTK) {
		node->token1 = tk;			//consume operation
		tk = scanner();

		return node;
	}
	else if (tk.type == EQEQTK) {
		node->token1 = tk;			//consume operation
		tk = scanner();

		return node;
	}
	else if (tk.type == LEFTBRACETK) {
		tk = scanner();

		if (tk.type == EQEQTK) {
			node->token1 = tk;		//consume operation
			tk = scanner();

			if (tk.type == RIGHTBRACETK) {
				tk = scanner();
				return node;

			}
			else {
				error(tk, RIGHTBRACETK);		//expected RIGHTBRACKET token
				return NULL;
			}
		}
		else {
			error(tk, EQEQTK);
			return NULL;
		}
	}
	else if (tk.type == MODULUSTK) {
		node->token1 = tk;			//consume operation
		tk = scanner();
		return node;

	}
	else {
		error(tk, RELATIONALOPERATORTK);
		return NULL;
	}

	return NULL;
}


//<goto> -> jump Identifier
Node* gotoFunc() {

	//std::cout << "goto called\n";

	Node* node = getNode(GOTOFUNCNODE);

	if (tk.type == IDTK) {
		node->token1 = tk;		//consume ID token
		tk = scanner();

		return node;
	}
	else {
		error(tk, IDTK);		//expected ID token
		return NULL;
	}
}


//<label> -> label Identifier
Node* label() {
	//std::cout << "label called\n";
	Node* node = getNode(LABELNODE);

	if (tk.type == IDTK) {
		node->token1 = tk;		//consume ID token
		tk = scanner();
		return node;
	}
	else {
		error(tk, IDTK);		//expected ID token
		return NULL;
	}
}


void error(token errorTk, int expectedToken) {
	std::cout << "Parser Error: There is an error! \n";
	std::cout << "Parser Error: ERROR in tk " << expectedTokens[errorTk.type] << " expected " << expectedTokens[expectedToken] <<
		" in line " << errorTk.lineNum << "\n";
	exit(1);
}
