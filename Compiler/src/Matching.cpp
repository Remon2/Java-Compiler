/*
 * Matching.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: Naruto
 */

#include "Matching.h"

DFA* startState; // DFA Start
DFA* lastACState; // Last Accepted DFA State
int lastACChar; // Last Accepted DFA State
DFA* currentState; // Current DFA State
vector<Token> tokens; //
string input;
string remainingInput;
unsigned int currentChar;
#include <fstream>

string getInput(char*pathOfFile = "code_input.txt") {
	ifstream in_stream;
	string line;
	string result;
	in_stream.open(pathOfFile);
	while (getline(in_stream, line)) {
		result += line+" ";
	}
	in_stream.close();
	return result;
}

//if (currentState->accepted) { // AC State
//	cout << "--- ACCEPTED ---" << endl;
//	lastACState =  currentState;
//	lastACChar = index;
//	currentState = currentState->to->at(c);
//	currentChar++;
//} else
Token t;
void move(char c, int index) {
//	cout << "%%%%%% " << currentState->name << endl;
	if (c == ' ') {
		cout << "--- Char= Space" << " --- Index= " << currentChar << endl;
	} else if (c == '\t') {
		cout << "--- Char= Tab" << " --- Index= " << currentChar << endl;
	} else {
		cout << "--- Char= " << c << " --- Index= " << currentChar << endl;
	}
	if ((c == ' ' || c == '\t' || c == '\n') && index == 0) { // Char = ' '
		remainingInput = remainingInput.substr(1);
		currentChar = 0;
		cout << "*** Remaining Input= ";
		cout << remainingInput << endl;

	} else if (currentState->to->find((char) 257) != currentState->to->end()) { // Found Epsilon
		if (currentState->accepted) { // AC State
			cout << "--- ACCEPTED ---" << currentState->token << endl;
			lastACState = currentState;
			lastACChar = index;
//			currentState = currentState->to->at(c);
		}
		currentState = currentState->to->at((char) 257);
	} else if (currentState->to->find(c) != currentState->to->end()) { // Found
		cout << "--- Char " << c << " Found ---" << endl;
		if (currentState->accepted) { // AC State
			cout << "--- ACCEPTED ---" << currentState->token << endl;
			lastACState = currentState;
			lastACChar = index;
//			currentState = currentState->to->at(c);
		}
		currentState = currentState->to->at(c);
		currentChar++;
	} else { //Not Found
		cout << "--- Char " << c << " NOT Found ---" << endl;
		if (currentState->accepted) { // AC State
			cout << "--- ACCEPTED ---" << currentState->token << endl;
			lastACState = currentState;
			lastACChar = index;
//			currentState = currentState->to->at(c);
		}
		if (currentState != startState && lastACState != startState) {
			string lexeme = remainingInput.substr(0, index);
			cout << "--- Lexeme= " << lexeme << " --- Token= "
					<< lastACState->token << endl;
			tokens.push_back(Token(lastACState->token, lexeme));
			t = Token(lastACState->token, lexeme);
//			cout << "^^^ Remaining Input= ";
//			cout << remainingInput << endl;
			remainingInput = remainingInput.substr(index);

//			cout << "### Remaining Input= ";
//			cout << remainingInput << endl;

			currentState = startState;
			lastACState = startState;
			currentChar = 0;
			lastACChar = 0;
//			return;
		} else {
			string lexeme = remainingInput.substr(0, 1);
//			cout << "--- Lexeme= " << lexeme << " --- Token= "
//					<< lastACState->token << endl;
			remainingInput = remainingInput.substr(1);
			tokens.push_back(Token("Lexeme Error", lexeme));
			t = Token("Lexeme Error", lexeme);
			cout << "***** ERROR *****" << endl;

			currentState = startState;
			lastACState = startState;
			currentChar = 0;
			lastACChar = 0;
//			return;
		}
	}
	cout << "\n";
}

vector<Token> match(DFA* start, string filePath) {
	startState = start;
	currentState = start;
	lastACState = start;
	input = getInput();

	remainingInput = input;
	cout << remainingInput << endl;
	lastACChar = 0;
	currentChar = 0;
	while (remainingInput.size() != 0) {
		move(remainingInput[currentChar], currentChar);
	}
	return tokens;
}
void initMatching(DFA* start, string filePath) {
	startState = start;
	currentState = start;
	lastACState = start;
	input = getInput();
	remainingInput = input;
	lastACChar = 0;
	currentChar = 0;
}
Token x;
Token getNextToken() {
	cout << "+++ Remaining Input= " << remainingInput << endl;
	while (remainingInput.size() != 0) {
		move(remainingInput[currentChar], currentChar);
		if (t.identifier != "Lexeme Error" && t.identifier != "") {
			x = t;
			t = Token("", "");
			return x;
		}
	}
	return Token("", "");
}
