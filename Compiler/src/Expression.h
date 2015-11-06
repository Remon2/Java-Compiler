/*
 * Expression.h
 *
 *  Created on: Mar 13, 2015
 *      Author: Naruto
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "State.h"
#include "Utilities.h"
#include "Parser.h"
#include <vector>
#include <iostream>
using namespace std;
class Expression {
public:
	Expression();
	Expression(State* state, int type);
	Expression(State* start, Expression* end1, Expression* end2);
	Expression(State* state, vector<State**> next);
	Expression(State* state, State* state2);
	Expression(State* state, Expression* end);

	Expression copy(Expression e);

	virtual ~Expression();
	State* start;
	vector<State**> next;
	int size;

private:
	void traverse(State* s);
	void deleteExpression(State *state);
};

#endif /* EXPRESSION_H_ */
