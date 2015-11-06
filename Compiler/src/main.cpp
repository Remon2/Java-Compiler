//============================================================================
// Name        : Lexical-Analyzer.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include "PostfixToNFA.h"
#include "Expression.h"
#include "parser.h"
#include "DFA.h"
#include "Matching.h"
#include "Storage.h"
#include "LeftRecursionEleminationAndLeftFactoring.h"
#include "ParsingTableGeneratorAndMatching.h"
using namespace std;

void printTokens(const vector<Token>& matchedTokens) {
	ofstream out("tokens.txt");
	cout << "*-----------------------*" << endl;
	cout << "|Lexeme\t|Identifier\t|" << endl;
	cout << "|-----------------------|" << endl;
	out << "*-----------------------*" << endl;
	out << "|Lexeme\t|Identifier\t|" << endl;
	out << "|-----------------------|" << endl;
	for (unsigned int i = 0; i < matchedTokens.size(); ++i) {
		string last = "\t\t|";
		if (matchedTokens[i].identifier.length() > 5) {
			last = "\t|";
		}
		cout << "| " << matchedTokens[i].lexeme << "\t| "
				<< matchedTokens[i].identifier << last << endl;
		out << "| " << matchedTokens[i].lexeme << "\t| "
				<< matchedTokens[i].identifier << last << endl;
	}
	cout << "*-----------------------*" << endl;
	out << "*-----------------------*" << endl;
}
//void printSymbolTable(const vector<Token>& matchedTokens) {
//	ofstream out("tokens.txt");
//	cout << "*-----------------------*" << endl;
//	cout << "|Symbol Table|" << endl;
//	cout << "|-----------------------|" << endl;
//	out << "*-----------------------*" << endl;
//	out << "|Lexeme\t|Identifier\t|" << endl;
//	out << "|-----------------------|" << endl;
//	for (unsigned int i = 0; i < matchedTokens.size(); ++i) {
//		string last = "\t\t|";
//		if (matchedTokens[i].identifier.length() > 5) {
//			last = "\t|";
//		}
//		if (matchedTokens[i].identifier == "id") {
//
//			cout << "| " << matchedTokens[i].lexeme << last << endl;
//			out << "| " << matchedTokens[i].lexeme << last << endl;
//		}
//	}
//	cout << "*-----------------------*" << endl;
//	out << "*-----------------------*" << endl;
//}
void printSymbolTable(const vector<Token>& matchedTokens) {
	ofstream out("output.txt");
	for (unsigned int i = 0; i < matchedTokens.size(); ++i) {
		if (matchedTokens[i].identifier == "punctuations"
				|| matchedTokens[i].identifier == "reserved"
				|| matchedTokens[i].identifier == "assign") {

			cout << matchedTokens[i].lexeme << endl;
			out << matchedTokens[i].lexeme << endl;
		} else {
			cout << matchedTokens[i].identifier << endl;
			out << matchedTokens[i].identifier << endl;
		}
	}
}

vector<vector<string> > postfixInput;
vector<Token> matchedTokens;
State* nfaStart;
DFA* dfaStart;
string codeInput = "code_input.txt";
int testLoad() {
	cout << "************************START STORE*****************" << endl;
	vector<DFA*> x = *getStorageGraph();
	cout << "*** " << x.size() << " ***" << endl;
	for (int i = 0; i < x.size(); i++) {
		cout << "*** " << x[i]->to->size() << endl;
	}
	cout << "***********************END STORE*****************" << endl;
	matchedTokens = match(x[0], codeInput);
	printTokens(matchedTokens);
	return 0;
}

int main() {

//	return testLoad();
	/*
	 * Get and Parse input
	 */
	postfixInput = *get_Postfix();
	cout << "*******************START PARSING******************" << endl;
	print_vector_2d(&postfixInput);
	cout << "*******************-----------------******************" << endl;
	/*
	 * Covert to NFA
	 */
	cout << "*******************START NFA******************" << endl;
	nfaStart = convertToNFA(postfixInput);
//	printState(nfaStart);

	/*
	 * Convert to DFA and Minimize it
	 */

	cout << "*******************START DFA******************" << endl;
	dfaStart = dfaStart->nfaToDFA(nfaStart);
	vector<DFA*> dfaVector = *(dfaStart->getAllDFA());

	cout << "*** " << dfaVector.size() << " ***" << endl;
	for (int i = 0; i < dfaVector.size(); i++) {
		cout << "*** " << dfaVector[i]->to->size() << endl;
	}
	/*
	 * Match input code
	 */
	cout << "*******************START MATCHING******************" << endl;
	initMatching(dfaStart, codeInput);
//	matchedTokens = match(dfaStart, codeInput);
	cout << "*******************END MATCHING******************" << endl;

	printTokens(matchedTokens);
	printSymbolTable(matchedTokens);
	cout << "*******************STORE GRAPH******************" << endl;
//	storeGraph();
	cout << "*******************END OF PHASE 1******************" << endl;
//****************************************************************************
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	cout << "*********************** START PHASE II *******************"
			<< endl;

	/*
	 * Read and Parse Grammar
	 */
	cout << "*********************** START PARSING *******************" << endl;
	parse();

	/*
	 * Left Recursion Elimination and Left Factoring
	 */
	cout << "******************* START ELIMINATION *******************" << endl;
	leftRecursionEleminationAndLeftFactoring();
	cout << "***************** START FIRST & FOLLOW ******************" << endl;
	/*
	 * First and Follow
	 */
	getFirst();
	getFollow();
	generateParsingTable();

	cout << "******************* START MATCHING *******************" << endl;
	/*
	 * Matching
	 */

	match();
	cout << "******************* END OF PHASE II *******************" << endl;
	return 0;
}

