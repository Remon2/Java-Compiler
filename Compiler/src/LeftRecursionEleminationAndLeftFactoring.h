/*
 * LeftRecursionEleminationAndLeftFactoring.h
 *
 *  Created on: Apr 8, 2015
 *      Author: Naruto
 */

#ifndef LEFTRECURSIONELEMINATIONANDLEFTFACTORING_H_
#define LEFTRECURSIONELEMINATIONANDLEFTFACTORING_H_
//#include "Utilities.h"
#include "Rule.h"
#include "CFGParser.h"
#include <vector>
#include <algorithm>
#include <set>
bool leftFactor(Rule* nonTerminal, int n);
void allLeftFactor();
void leftRecursionEleminationAndLeftFactoring();
#endif /* LEFTRECURSIONELEMINATIONANDLEFTFACTORING_H_ */
