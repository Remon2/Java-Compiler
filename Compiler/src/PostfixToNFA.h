/*
 * postfixToNFA.h
 *
 *  Created on: Mar 14, 2015
 *      Author: Naruto
 */

#ifndef POSTFIXTONFA_H_
#define POSTFIXTONFA_H_

#include "Expression.h"
#include <stack>
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

void doOperation(string token, char c, Expression* tokenE);
Expression postfixToNFA(vector<string> postfix);
State* convertToNFA(vector<vector<string> > postfix);
//void printNFA(Expression e);
void printState(State* s);
//void printMap();
#endif /* POSTFIXTONFA_H_ */
