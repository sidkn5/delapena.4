/*Student: Sean Dela Pena
* File: scanner.cpp
* 
* This file contains most of the functionality of the scanner.
* This file also contains the testscanner that calls the fsaDriver
* and print the tokens.
* 
* You will find the FSA Table and FSA driver here.
*/
#include "scanner.hpp"
#include "token.hpp"
#include "character.hpp"


const int rowSize = 26;
const int colSize = 25;

int specificKeyword = 0;
int charNumber = 0;
bool startComment = false;
bool endComment = false;

enum states {
	ERROR = -1, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11,
	s12, s13, s14, s15, s16, s17, s18, s19, s20, s21, s22, s23,
	s24, s25,
	FINAL = 101
};

std::string keywords[] = {
	"start", "stop", "loop", "while", "for", "label", "exit", "listen", "talk", "program", "if", "then", "assign", "declare", "jump", "else"
};

//std::string tokenNames[]
std::string tokenNames[] = {
	"HolderTk", "IDTk", "INTEGERTk", "WSTk", "EQUALTk", "EQEQTk", "GREATERTHANTk", "LESSTHANTk", "COLONTk",
	"COLONEQTk", "PLUSTk", "MINUSTk", "MULTIPLYTk", "DIVIDETk", "MODULUSTk", "DOTTk", "LEFTPARENTK", 
	"RIGHTPARENTk", "COMMATk", "LEFTBRACETk", "RIGHTBRACETk", "SEMICOLONTk", "LEFTBRACKETTk", "RIGHTBRACKETTk",
	"EOFTk", "KEYWORDTk", "STARTTk", "STOPTk", "LOOPTk", "WHILETk", "FORTk", "LABELTk", "EXITTk", "LISTENTk",
	"TALKTk", "PROGRAMTk", "IFTk", "THENTk", "ASSIGNTk", "DECLARETk", "JUMPTk", "ELSETk"
};

int fsaTable[rowSize][colSize] = {
	//  a-z   A-Z   0-9     $    WS     =     >     <     :     +	   -	  *      /	   %	   .	  (	     )	    ,	   {	  }	;     [       ]    EOF
	{   s2,	ERROR,   s3,   s2,   s4,   s5,   s7,   s8,   s9,  s11,   s12,   s13,   s14,   s15,   s16,   s17,   s18,   s19,   s20,   s21,   s22,   s23,   s24,   s25, FINAL},	//s1
	{   s2,    s2,   s2,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s2
	{FINAL, FINAL,   s3,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s3
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s4
	{FINAL, FINAL,FINAL,FINAL,FINAL,   s6,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s5
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s6
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s7
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s8
	{FINAL, FINAL,FINAL,FINAL,FINAL,  s10,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s9
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s10
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s11
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s12
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s13
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s14
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s15
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s16
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s17
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s18
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s19
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s20
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s21
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s22
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s23
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s24
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s25
	{FINAL, FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL, FINAL},	//s26

};

std::map<char, int> columnInt = {
	{'a', LOWERCASE},       //a-z   lowercase
	{'A', UPPERCASE},       //A-Z   uppercase
	{'1', INTEGER},       //0-9 digit
	{'$', DOLLAR},
	{' ', WS},       //WS
	{'=', EQUAL},
	{'>', GREATERTHAN},
	{'<', LESSTHAN},
	{':', COLON},
	{'+', PLUS},
	{'-', MINUS},
	{'*', MULTIPLY},
	{'/', DIVIDE},
	{'%', MODULUS},
	{'.', DOT},
	{'(', LEFTPAREN},
	{')', RIGHTPAREN},
	{',', COMMA},
	{'{', LEFTBRACE},
	{'}', RIGHTBRACE},
	{';', SEMICOLON},
	{'[', LEFTBRACKET},
	{']', RIGHTBRACKET}, {EOF, 23},      //EOF
	{'\0',24} 				//end of a string
};


std::vector<token> testScanner(std::string filename) {
	
	std::vector <token> tks = fsaDriver(filename);	
	cleanVector(tks);
	//printTokenVector(tks);
	return tks;
}

void cleanVector(std::vector<token>& tks) {
	for (int i = 0; i < tks.size(); i++) {
		if (tks[i].type == HOLDERTK || tks[i].type == WSTK || tks[i].tokenString == "") {
			//since we considered WS as a tk to help with the FSA TABLE our vector will
			//have WStks but we don't need it so delete them
			tks.erase(tks.begin() + i);
			i--;
		}
		if (tks[tks.size()].lineNum == tks[i].lineNum && tks[i].type != EOFTK) {
			tks.erase(tks.begin() + i);
			i--;
		}
	}
	
}


//prints the vector
void printTokenVector(std::vector<token> &tks) {

	for (int i = 0; i < tks.size(); i++) {
		/*if (tks[i].type == HOLDERTK || tks[i].type == WSTK || tks[i].tokenString == "") {
			//since we considered WS as a tk to help with the FSA TABLE our vector will
			//have WStks but we don't need it so delete them
			//might need to do this before taking the tokens for the next project
			//move to its own function
			tks.erase(tks.begin() + i);
			i--;
		}*/
		//else {
			std::cout << "[" << tokenNames[tks[i].type] << ", \"" << tks[i].tokenString
				<< "\", line " << tks[i].lineNum << ":" << tks[i].charNum << "]" << std::endl;
		//}
	}
}


void printError(std::string str, int line, int charNumber) {
	std::cout << "This is an error state, terminating...\n";
	std::cout << "ERORR SCANNER: Error in character " << "\"" << str << "\"" << " line: " << line << ":" << charNumber << "\n";
	
	exit(1);
}


//this function filters line and removes the comments, also checks for invalid characters
LinesContainer filter(std::string filename, int lineNumber) {
	std::ifstream file;
	LinesContainer buffer;
	int line = 0;
	file.open(filename);

	while (lineNumber) {	
		if (std::getline(file, buffer.value)) {			
			line++;
			lineNumber--;
			buffer.value.insert(0, 1, ' ');

			//remove the comments, and check for invalid characters
			for (int i = 0; i < buffer.value.size(); i++) {

				if (startComment == true && buffer.value[i] == '&' && buffer.value[i + 1] == '&') {
					startComment = false;
					i++;
					break;
				}
				else if (startComment) {
					buffer.value[i] = ' ';
				}
				else if (buffer.value[i] == '&' && buffer.value[i + 1] == '&') {
					startComment = true;
					i++;
				}
			}
		
			if (lineNumber <= 0) break;
		}
		else {
			buffer.endOfFile = true;		//end of file reached
			break;
		}

		
	}
	replace(buffer.value.begin(), buffer.value.end(), '&', ' ');
	startComment = false;		
	buffer.lineNum = line;
	return buffer;
}

//FSA DRIVER, this function takes in the filename, filters the file, and tokenizes the 
//input line by line
//returns a vector of tokens that can be printed
std::vector<token> fsaDriver(std::string filename) {
	

	//pseudo from powerpoint
	LinesContainer currentLine;
	std::vector <token> allTokens;
	int state = s1;
	int nextState;
	token newToken;
	char nextChar;			//+1 lookahead
	std::string word = "";
	int charColumn;
	int tokenIntType;
	int previousState;		//might be helpful?
	int currentLineNum = 1;

	
	//go through the file until you get EOF
	while (!currentLine.endOfFile) {

		currentLine = filter(filename, currentLineNum);			//get that line from the file and clean it
		int length = currentLine.value.size();
		charNumber = 0;											//character number counter per line
		for (int i = 0; i < length; i++) {
			charColumn = getIntFsa(currentLine.value[i], currentLineNum, charNumber);
			nextState = fsaTable[state][charColumn];
			charNumber = i-1;
			
			if (nextState <= ERROR || nextState > FINAL || state == ERROR) {
				printError(word, currentLineNum, charNumber);		//somthing wrong with input, terminatei
				
			}
			else if (nextState == FINAL) {				
			
				specificKeyword = 0;
				if (checkKeyword(word)) {
					//tokenize
					allTokens.push_back(getToken(tokenType(KEYWORDTK + specificKeyword), word, currentLineNum, (charNumber-word.size()+1)));
					word = "";
					state = s1;
					nextState = fsaTable[state][charColumn];
					
					word.append(1, currentLine.value[i]);
					if (nextState != FINAL) {
						state = nextState;
					}
				}
				else {
						tokenIntType = state;
						allTokens.push_back(getToken(tokenType(tokenIntType), word, currentLineNum, (charNumber - word.size() + 1)));
						word = "";
						state = s1;
						nextState = fsaTable[state][charColumn];
						word.append(1, currentLine.value[i]);

						if (nextState != FINAL) {
							state = nextState;
						}
				}
				
			}
			else {
				nextChar = currentLine.value[i];
				previousState = state;
				state = nextState;
				word.append(1, nextChar);
				nextChar = currentLine.value[i + 1];
				charColumn = getIntFsa(nextChar,currentLineNum, charNumber);
			}

		}

		//after going through the line there will be one last string the is stored in word, tokenize it.
		//check if last is keyword
		
		nextState = fsaTable[state][charColumn];
		specificKeyword = 0;
		if (checkKeyword(word)) {
			allTokens.push_back(getToken(tokenType(KEYWORDTK + specificKeyword), word, currentLineNum, (charNumber - word.size() + 2)));
			word = "";		//reset the word after tokenizing
		}
		else {
			int n = charNumber - word.size();
			if (n <= 0) { 
				charNumber ++;	//since we counted WS as a character, we need to increment
			}
			if (nextState <= ERROR || nextState > FINAL || state == ERROR) {
		
				printError(word, currentLineNum, charNumber);		//somthing wrong with input, terminate
			}
			
			allTokens.push_back(getToken(tokenType(state), word, currentLineNum, (charNumber - word.size()) + 1));
			word = "";		//reset the word after tokenizing
		}

		currentLineNum++;
	}

	//when endOfFile is true add an EOFTK at the end
	
	allTokens.push_back(getToken(EOFTK, "EOF", currentLineNum-1, 0));

	return allTokens;
}

//will get the fsa table column int
int getIntFsa(char ch, int l, int n) {

	int z = 101;
	//the symbols are in a map
	int temp = columnInt[ch];
	if (islower(ch)) {
		return LOWERCASE;
	}
	else if (isupper(ch)) {
		return UPPERCASE;
	}
	else if (isspace(ch)) {
		return WS;
	}
	else if (isdigit(ch)) {
		return INTEGER;
	}else if (temp){
		return temp;
	}
	else {
		
		std::cout << "ERROR SCANNER: This is an invalid character " << ch << ".\n";
		printError(std::string(1,ch), l,n+2); //added 2 because we consider whitespaces
		return n;
	}
}


//checks wheter the word in its final state is a keyword, 
//increment to get the specific keyword
bool checkKeyword(std::string str) {
	int keywordNum = 16;
	
	token temp;
	std::string keywordStrings[16] = {"start", "stop", "loop", "while", "for", "label", "exit", "listen",
		"talk", "program", "if", "then", "assign", "declare", "jump", "else" };

	for (int i = 0; i < keywordNum; i++) {
		specificKeyword++;
		if (str == keywordStrings[i]) {
			return true;
		}
	}
	return false;
}

//tokenizer
token getToken(tokenType type, std::string value, int lineNum, int charNum) {
	token tokenStr;
	tokenStr.type = type;
	tokenStr.tokenString = value;
	tokenStr.lineNum = lineNum;
	tokenStr.charNum = charNum;

	return tokenStr;
}
