/*
 * Matching.h
 *
 *  Created on: Mar 17, 2015
 *      Author: Naruto
 */

#ifndef MATCHING_H_
#define MATCHING_H_
#include "DFA.h"
#include "Token.h"
/*
 * Read from file
 * Remove spaces, \n, \t
 * trace the input
 */

void move(char c, int index);
vector<Token> match(DFA* state, string filePath);
Token getNextToken();
void initMatching(DFA* start, string filePath) ;
//string getInput();
#endif /* MATCHING_H_ */
