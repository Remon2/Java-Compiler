/*
 * State.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: Naruto
 */

#include "State.h"
static int getID = 0;

State::State(char c, State *state1, State *state2, bool split, bool accepted,
		string token) {
	this->id = getID++;
	this->c = c;

	this->next = state1;
	this->next2 = state2;

	this->visited = false;
	this->split = split;

	this->accepted = accepted;
	this->token = token;
	this->epsilon = false;
//	this->reserved = false;
	this->priority = 100000;
}

State::State(char c, State *state1, State *state2, bool split) {
	this->id = getID++;
	this->c = c;

	this->next = state1;
	this->next2 = state2;

	this->visited = false;
	this->split = split;

	this->accepted = false;
	this->token = "";
	this->epsilon = false;
//	this->reserved = false;
	this->priority = 100000;
}

State::State() {
	this->c = '^';
	this->id = getID++;
	this->next = NULL;
	this->next2 = NULL;
	this->visited = false;
	this->split = false;
	this->accepted = false;
	this->token = "";
	this->epsilon = false;
//	this->reserved = false;
	this->priority = 100000;
}
State::~State() {
	delete(next);
	delete(next2);
}

