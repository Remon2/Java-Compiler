/*
 * State.h
 *
 *  Created on: Mar 13, 2015
 *      Author: Naruto
 */

#ifndef STATE_H_
#define STATE_H_

#include "Utilities2.h"
#include <string>
//#include <vector>
#include <iostream>
//#include <map>
using namespace std;
class State {
public:
	State(char c, State *state1, State *state2, bool split, bool accepted,
			string token);
	State(char c, State *state1, State *state2, bool split);
	State();

//	copy (State);
	virtual ~State();

	char c;
	bool visited;
	bool accepted;
	bool split;
	bool epsilon;
	int id;
	int priority;
	string token;
	State* next;
	State* next2;
//private:
};

#endif /* STATE_H_ */
